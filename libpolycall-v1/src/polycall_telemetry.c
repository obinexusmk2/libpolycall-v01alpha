#include "polycall_telemetry.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include <sys/time.h>
#endif

typedef struct {
    bool initialized;
    bool stream_enabled;
    FILE* export_file;
    char export_path[256];
    uint64_t emitted_events;
    pthread_mutex_t lock;
} PolyCallTelemetryRuntime;

static PolyCallTelemetryRuntime g_telemetry = {0};

static uint64_t now_ms(void) {
#ifdef _WIN32
    return (uint64_t)time(NULL) * 1000ULL;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec * 1000ULL) + ((uint64_t)tv.tv_usec / 1000ULL);
#endif
}

static void json_escape(const char* input, char* output, size_t output_size) {
    if (!output || output_size == 0) {
        return;
    }
    if (!input) {
        output[0] = '\0';
        return;
    }

    size_t o = 0;
    for (size_t i = 0; input[i] != '\0' && o + 2 < output_size; i++) {
        if (input[i] == '\\' || input[i] == '"') {
            output[o++] = '\\';
            output[o++] = input[i];
        } else if (input[i] == '\n') {
            output[o++] = '\\';
            output[o++] = 'n';
        } else if ((unsigned char)input[i] >= 0x20) {
            output[o++] = input[i];
        }
    }
    output[o] = '\0';
}

const char* polycall_telemetry_event_type_to_string(polycall_telemetry_event_type_t type) {
    switch (type) {
        case POLYCALL_TELEMETRY_CONNECTION: return "connection";
        case POLYCALL_TELEMETRY_TRANSITION: return "transition";
        case POLYCALL_TELEMETRY_INTEGRITY: return "integrity";
        case POLYCALL_TELEMETRY_CONSENSUS: return "consensus";
        default: return "unknown";
    }
}

const char* polycall_decision_state_to_string(polycall_decision_state_t state) {
    switch (state) {
        case POLYCALL_DECISION_YES: return "YES";
        case POLYCALL_DECISION_NO: return "NO";
        case POLYCALL_DECISION_MAYBE: return "MAYBE";
        default: return "MAYBE";
    }
}

bool polycall_telemetry_init(void) {
    if (g_telemetry.initialized) {
        return true;
    }

    if (pthread_mutex_init(&g_telemetry.lock, NULL) != 0) {
        return false;
    }

    g_telemetry.initialized = true;
    g_telemetry.stream_enabled = false;
    g_telemetry.export_file = NULL;
    g_telemetry.export_path[0] = '\0';
    g_telemetry.emitted_events = 0;
    return true;
}

void polycall_telemetry_shutdown(void) {
    if (!g_telemetry.initialized) {
        return;
    }

    pthread_mutex_lock(&g_telemetry.lock);
    if (g_telemetry.export_file) {
        fclose(g_telemetry.export_file);
        g_telemetry.export_file = NULL;
    }
    g_telemetry.initialized = false;
    pthread_mutex_unlock(&g_telemetry.lock);
    pthread_mutex_destroy(&g_telemetry.lock);
}

bool polycall_telemetry_enable_stream(bool enabled) {
    if (!g_telemetry.initialized) {
        return false;
    }

    pthread_mutex_lock(&g_telemetry.lock);
    g_telemetry.stream_enabled = enabled;
    pthread_mutex_unlock(&g_telemetry.lock);
    return true;
}

bool polycall_telemetry_set_export_path(const char* path) {
    if (!g_telemetry.initialized || !path || path[0] == '\0') {
        return false;
    }

    FILE* file = fopen(path, "a");
    if (!file) {
        return false;
    }

    pthread_mutex_lock(&g_telemetry.lock);
    if (g_telemetry.export_file) {
        fclose(g_telemetry.export_file);
    }
    g_telemetry.export_file = file;
    strncpy(g_telemetry.export_path, path, sizeof(g_telemetry.export_path) - 1);
    g_telemetry.export_path[sizeof(g_telemetry.export_path) - 1] = '\0';
    pthread_mutex_unlock(&g_telemetry.lock);

    return true;
}

void polycall_telemetry_get_status(polycall_telemetry_status_t* status) {
    if (!status) {
        return;
    }

    memset(status, 0, sizeof(*status));
    if (!g_telemetry.initialized) {
        return;
    }

    pthread_mutex_lock(&g_telemetry.lock);
    status->stream_enabled = g_telemetry.stream_enabled;
    status->export_enabled = (g_telemetry.export_file != NULL);
    strncpy(status->export_path, g_telemetry.export_path, sizeof(status->export_path) - 1);
    status->emitted_events = g_telemetry.emitted_events;
    pthread_mutex_unlock(&g_telemetry.lock);
}

bool polycall_telemetry_emit(const polycall_telemetry_event_t* event) {
    if (!g_telemetry.initialized || !event) {
        return false;
    }

    char component[128], action[128], status[128], subject[256], details[512];
    json_escape(event->component ? event->component : "", component, sizeof(component));
    json_escape(event->action ? event->action : "", action, sizeof(action));
    json_escape(event->status ? event->status : "", status, sizeof(status));
    json_escape(event->subject ? event->subject : "", subject, sizeof(subject));
    json_escape(event->details ? event->details : "", details, sizeof(details));

    uint64_t ts = event->timestamp_ms ? event->timestamp_ms : now_ms();
    const char* type = polycall_telemetry_event_type_to_string(event->event_type);
    const char* decision = polycall_decision_state_to_string(event->decision_state);

    char line[1600];
    int written = snprintf(
        line,
        sizeof(line),
        "{\"timestamp_ms\":%llu,\"event_type\":\"%s\",\"component\":\"%s\",\"action\":\"%s\",\"decision_state\":\"%s\",\"status\":\"%s\",\"subject\":\"%s\",\"from_state\":%d,\"to_state\":%d,\"checksum\":%u,\"consensus_ratio\":%.3f,\"details\":\"%s\"}\n",
        (unsigned long long)ts,
        type,
        component,
        action,
        decision,
        status,
        subject,
        event->from_state,
        event->to_state,
        event->checksum,
        event->consensus_ratio,
        details
    );

    if (written <= 0) {
        return false;
    }

    pthread_mutex_lock(&g_telemetry.lock);
    if (g_telemetry.stream_enabled) {
        fputs(line, stdout);
        fflush(stdout);
    }
    if (g_telemetry.export_file) {
        fputs(line, g_telemetry.export_file);
        fflush(g_telemetry.export_file);
    }
    g_telemetry.emitted_events++;
    pthread_mutex_unlock(&g_telemetry.lock);
    return true;
}
