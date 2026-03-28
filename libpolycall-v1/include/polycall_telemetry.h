#ifndef POLYCALL_TELEMETRY_H
#define POLYCALL_TELEMETRY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    POLYCALL_DECISION_YES = 0,
    POLYCALL_DECISION_NO = 1,
    POLYCALL_DECISION_MAYBE = 2
} polycall_decision_state_t;

typedef enum {
    POLYCALL_TELEMETRY_CONNECTION = 0,
    POLYCALL_TELEMETRY_TRANSITION = 1,
    POLYCALL_TELEMETRY_INTEGRITY = 2,
    POLYCALL_TELEMETRY_CONSENSUS = 3
} polycall_telemetry_event_type_t;

typedef struct {
    uint64_t timestamp_ms;
    polycall_telemetry_event_type_t event_type;
    polycall_decision_state_t decision_state;
    const char* component;
    const char* action;
    const char* status;
    const char* subject;
    int from_state;
    int to_state;
    uint32_t checksum;
    double consensus_ratio;
    const char* details;
} polycall_telemetry_event_t;

typedef struct {
    bool stream_enabled;
    bool export_enabled;
    char export_path[256];
    uint64_t emitted_events;
} polycall_telemetry_status_t;

bool polycall_telemetry_init(void);
void polycall_telemetry_shutdown(void);

bool polycall_telemetry_enable_stream(bool enabled);
bool polycall_telemetry_set_export_path(const char* path);
bool polycall_telemetry_emit(const polycall_telemetry_event_t* event);
void polycall_telemetry_get_status(polycall_telemetry_status_t* status);

const char* polycall_telemetry_event_type_to_string(polycall_telemetry_event_type_t type);
const char* polycall_decision_state_to_string(polycall_decision_state_t state);

#ifdef __cplusplus
}
#endif

#endif // POLYCALL_TELEMETRY_H
