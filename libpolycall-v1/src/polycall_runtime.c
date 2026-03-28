#include "polycall_runtime.h"
#include "polycall_protocol.h"
#include "polycall_tokenizer.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PPI_VERSION "1.0.0"

typedef bool (*CommandHandler)(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3);

typedef struct {
    const char* name;
    CommandHandler handler;
    const char* usage;
} Command;

PPI_Runtime g_runtime = {0};

static bool cmd_init(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3);
static bool cmd_add_state(const PPI_Runtime* runtime, const char* name, const char* arg2, const char* arg3);
static bool cmd_help(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3);
static bool cmd_quit(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3);

static const Command COMMANDS[] = {
    {"init", cmd_init, "init"},
    {"add_state", cmd_add_state, "add_state NAME"},
    {"help", cmd_help, "help"},
    {"quit", cmd_quit, "quit"},
};

static void on_init(polycall_context_t ctx) { (void)ctx; printf("State callback: System initialized\n"); }
static void on_ready(polycall_context_t ctx) { (void)ctx; printf("State callback: System ready\n"); }
static void on_running(polycall_context_t ctx) { (void)ctx; printf("State callback: System running\n"); }
static void on_paused(polycall_context_t ctx) { (void)ctx; printf("State callback: System paused\n"); }
static void on_error(polycall_context_t ctx) {
    (void)ctx;
    printf("State callback: System error\n");
    g_runtime.running = false;
}

static void on_network_connect(NetworkEndpoint* endpoint) {
    if (!endpoint) {
        return;
    }

    printf("\nNew connection from %s:%d\n> ", endpoint->address, endpoint->port);
    fflush(stdout);
}

static void on_network_disconnect(NetworkEndpoint* endpoint) {
    if (!endpoint) {
        return;
    }

    printf("\nClient disconnected from %s:%d\n> ", endpoint->address, endpoint->port);
    fflush(stdout);
}

static void on_network_receive(NetworkEndpoint* endpoint, NetworkPacket* packet) {
    if (!endpoint || !packet || !packet->data) {
        return;
    }

    printf("\nReceived data from %s:%d: %.*s\n> ", endpoint->address, endpoint->port, (int)packet->size, (char*)packet->data);
    fflush(stdout);

    NetworkPacket response = {.data = packet->data, .size = packet->size, .flags = 0};
    net_send(endpoint, &response);
}

static void add_to_history(const char* command) {
    if (!command) {
        return;
    }

    size_t cmd_len = strlen(command);
    size_t copy_len = (cmd_len < MAX_INPUT - 1) ? cmd_len : MAX_INPUT - 1;

    if (g_runtime.history_count < HISTORY_SIZE) {
        memcpy(g_runtime.command_history[g_runtime.history_count], command, copy_len);
        g_runtime.command_history[g_runtime.history_count][copy_len] = '\0';
        g_runtime.history_count++;
        return;
    }

    memmove(&g_runtime.command_history[0],
            &g_runtime.command_history[1],
            (HISTORY_SIZE - 1) * sizeof(g_runtime.command_history[0]));
    memcpy(g_runtime.command_history[HISTORY_SIZE - 1], command, copy_len);
    g_runtime.command_history[HISTORY_SIZE - 1][copy_len] = '\0';
}

static void list_endpoints(void) {
    for (size_t i = 0; i < g_runtime.program_count; i++) {
        NetworkProgram* program = g_runtime.programs[i];
        if (!program || !program->endpoints) {
            continue;
        }

        printf("\nProgram %zu Endpoints:\n", i);
        for (size_t j = 0; j < program->count; j++) {
            NetworkEndpoint* ep = &program->endpoints[j];
            printf("  Endpoint %zu: %s:%d (%s)\n", j, ep->address, ep->port, ep->protocol == NET_TCP ? "TCP" : "UDP");
        }
    }
}

static void list_clients(void) {
    for (size_t i = 0; i < g_runtime.program_count; i++) {
        NetworkProgram* program = g_runtime.programs[i];
        if (!program) {
            continue;
        }

        printf("\nProgram %zu Clients:\n", i);
        pthread_mutex_lock(&program->clients_lock);
        for (int j = 0; j < NET_MAX_CLIENTS; j++) {
            pthread_mutex_lock(&program->clients[j].lock);
            if (program->clients[j].is_active) {
                printf("  Client %d: Connected\n", j);
            }
            pthread_mutex_unlock(&program->clients[j].lock);
        }
        pthread_mutex_unlock(&program->clients_lock);
    }
}

static void list_states(void) {
    if (!g_runtime.state_machine) {
        printf("State machine not initialized\n");
        return;
    }

    printf("\nStates:\n");
    for (unsigned int i = 0; i < g_runtime.state_machine->num_states; i++) {
        printf("  %u: %s (locked: %s)\n",
               i,
               g_runtime.state_machine->states[i].name,
               g_runtime.state_machine->states[i].is_locked ? "yes" : "no");
    }
}

static void list_transitions(void) {
    if (!g_runtime.state_machine) {
        printf("State machine not initialized\n");
        return;
    }

    printf("\nTransitions:\n");
    for (unsigned int i = 0; i < g_runtime.state_machine->num_transitions; i++) {
        printf("  %s: %u -> %u\n",
               g_runtime.state_machine->transitions[i].name,
               g_runtime.state_machine->transitions[i].from_state,
               g_runtime.state_machine->transitions[i].to_state);
    }
}

static void show_history(void) {
    printf("\nCommand History:\n");
    for (int i = 0; i < g_runtime.history_count; i++) {
        printf("  %d: %s\n", i + 1, g_runtime.command_history[i]);
    }
}

static void show_status(void) {
    printf("\nSystem Status:\n");
    printf("  State Machine: %s\n", g_runtime.state_machine ? "Initialized" : "Not initialized");
    printf("  Network Programs: %zu\n", g_runtime.program_count);
    printf("  Running: %s\n", g_runtime.running ? "Yes" : "No");

    if (g_runtime.state_machine) {
        printf("  Current State: %u\n", g_runtime.state_machine->current_state);
    }

    list_endpoints();
    list_clients();
}

bool runtime_start_network_service(uint16_t port) {
    if (g_runtime.program_count >= MAX_PROGRAMS) {
        fprintf(stderr, "Maximum network program count reached\n");
        return false;
    }

    NetworkProgram* program = calloc(1, sizeof(NetworkProgram));
    if (!program) {
        return false;
    }

    net_init_program(program);
    if (!program->endpoints || program->count == 0) {
        free(program);
        return false;
    }

    program->endpoints[0].port = port;
    program->handlers.on_receive = on_network_receive;
    program->handlers.on_connect = on_network_connect;
    program->handlers.on_disconnect = on_network_disconnect;

    g_runtime.programs[g_runtime.program_count++] = program;
    return true;
}

void runtime_stop_network_services(void) {
    for (size_t i = 0; i < g_runtime.program_count; i++) {
        if (g_runtime.programs[i]) {
            net_cleanup_program(g_runtime.programs[i]);
            free(g_runtime.programs[i]);
            g_runtime.programs[i] = NULL;
        }
    }
    g_runtime.program_count = 0;
}

static void runtime_process_command(PPI_Runtime* runtime, const char* input) {
    if (!runtime || !input) {
        return;
    }

    char buffer[MAX_INPUT];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* cmd = strtok(buffer, " ");
    char* arg1 = strtok(NULL, " ");
    char* arg2 = strtok(NULL, " ");
    char* arg3 = strtok(NULL, " ");

    if (!cmd) {
        return;
    }

    for (size_t i = 0; i < sizeof(COMMANDS) / sizeof(COMMANDS[0]); i++) {
        if (strcmp(cmd, COMMANDS[i].name) == 0) {
            if (!COMMANDS[i].handler(runtime, arg1, arg2, arg3)) {
                printf("Usage: %s\n", COMMANDS[i].usage);
            }
            return;
        }
    }

    printf("Unknown command. Type 'help' for available commands\n");
}

static bool cmd_init(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3) {
    (void)arg1;
    (void)arg2;
    (void)arg3;

    if (runtime->state_machine) {
        printf("State machine already initialized\n");
        return false;
    }

    if (polycall_sm_create_with_integrity(runtime->pc_ctx, &g_runtime.state_machine, NULL) != POLYCALL_SM_SUCCESS) {
        printf("Failed to initialize state machine\n");
        return false;
    }

    polycall_sm_add_state(g_runtime.state_machine, "INIT", on_init, NULL, false);
    polycall_sm_add_state(g_runtime.state_machine, "READY", on_ready, NULL, false);
    polycall_sm_add_state(g_runtime.state_machine, "RUNNING", on_running, NULL, false);
    polycall_sm_add_state(g_runtime.state_machine, "PAUSED", on_paused, NULL, false);
    polycall_sm_add_state(g_runtime.state_machine, "ERROR", on_error, NULL, true);

    printf("State machine initialized successfully\n");
    return true;
}

static bool cmd_add_state(const PPI_Runtime* runtime, const char* name, const char* arg2, const char* arg3) {
    (void)arg2;
    (void)arg3;

    if (!runtime->state_machine) {
        printf("State machine not initialized. Use 'init' first.\n");
        return false;
    }

    if (!name) {
        return false;
    }

    if (polycall_sm_add_state(runtime->state_machine, name, NULL, NULL, false) == POLYCALL_SM_SUCCESS) {
        printf("State '%s' added successfully\n", name);
        return true;
    }

    printf("Failed to add state\n");
    return false;
}

static bool cmd_help(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3) {
    (void)runtime;
    (void)arg1;
    (void)arg2;
    (void)arg3;

    printf("\nPolyCall CLI Commands:\n");
    printf("Network Commands:\n");
    printf("  start_network          - Start network services\n");
    printf("  stop_network           - Stop network services\n");
    printf("  list_endpoints         - List all network endpoints\n");
    printf("  list_clients           - List connected clients\n");

    printf("\nState Machine Commands:\n");
    printf("  init                   - Initialize the state machine\n");
    printf("  add_state NAME         - Add a new state\n");
    printf("  add_transition NAME FROM TO - Add a transition\n");
    printf("  execute NAME           - Execute a transition\n");

    printf("\nMiscellaneous Commands:\n");
    printf("  help                   - Show this help message\n");
    printf("  quit                   - Exit the program\n");
    return true;
}

static bool cmd_quit(const PPI_Runtime* runtime, const char* arg1, const char* arg2, const char* arg3) {
    (void)runtime;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    g_runtime.running = false;
    return true;
}

static bool configure_program_port(NetworkProgram* program, uint16_t port) {
    if (!program || !program->endpoints || program->count == 0) {
        return false;
    }

    program->endpoints[0].port = port;
    return true;
}

bool runtime_initialize(void) {
    runtime_register_signal_handlers();

#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        fprintf(stderr, "Failed to initialize Winsock\n");
        return false;
    }
    g_runtime.wsa_initialized = true;
#endif

    polycall_config_t config = {.flags = 0, .memory_pool_size = 1024 * 1024, .user_data = NULL};
    if (polycall_init_with_config(&g_runtime.pc_ctx, &config) != POLYCALL_SUCCESS) {
        fprintf(stderr, "Failed to initialize PolyCall context\n");
#ifdef _WIN32
        WSACleanup();
        g_runtime.wsa_initialized = false;
#endif
        return false;
    }

    g_runtime.state_machine = NULL;
    g_runtime.running = true;
    return true;
}

void runtime_cleanup(void) {
    directory_cleanup_control();
    runtime_stop_network_services();

    if (g_runtime.state_machine) {
        polycall_sm_destroy(g_runtime.state_machine);
        g_runtime.state_machine = NULL;
    }

    if (g_runtime.pc_ctx) {
        polycall_cleanup(g_runtime.pc_ctx);
        g_runtime.pc_ctx = NULL;
    }

#ifdef _WIN32
    if (g_runtime.wsa_initialized) {
        WSACleanup();
        g_runtime.wsa_initialized = false;
    }
#endif
}

void runtime_cleanup_and_exit(void) {
    runtime_cleanup();
    printf("Goodbye!\n");
    exit(0);
}

static void signal_handler(int signum) {
    (void)signum;
    runtime_cleanup_and_exit();
}

void runtime_register_signal_handlers(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
}

bool runtime_run_non_interactive(const char* config_file) {
    FILE* fp = fopen(config_file, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open config file: %s\n", config_file);
        return false;
    }

    char line[MAX_INPUT];
    bool network_started = false;
    uint16_t port_number = 8080;

    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (len == 0 || line[0] == '#') {
            continue;
        }

        char cmd[32];
        char value[256];
        if (sscanf(line, "%31s %255s", cmd, value) != 2) {
            continue;
        }

        if (strcmp(cmd, "port") == 0) {
            uint16_t host_port = 0;
            uint16_t container_port = 0;
            if (sscanf(value, "%hu:%hu", &host_port, &container_port) == 2) {
                port_number = container_port;
            }
            continue;
        }

        if (strcmp(cmd, "network") == 0 && strcmp(value, "start") == 0) {
            if (runtime_start_network_service(port_number)) {
                network_started = true;
                printf("Network services started on port %d\n", port_number);
            } else {
                fprintf(stderr, "Failed to start network services\n");
            }
        }
    }

    fclose(fp);

    if (!network_started) {
        fprintf(stderr, "Warning: No network services were started\n");
    }

    printf("Running in non-interactive mode...\n");
    g_runtime.running = true;

    while (g_runtime.running) {
        for (size_t i = 0; i < g_runtime.program_count; i++) {
            NetworkProgram* program = g_runtime.programs[i];
            if (program && configure_program_port(program, port_number)) {
                net_run(program);
            }
        }
        usleep(1000);
    }

    return true;
}

void runtime_run_interactive(void) {
    char input[MAX_INPUT];
    printf("PolyCall CLI v%s - Type 'help' for commands\n", PPI_VERSION);

    while (g_runtime.running) {
        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) {
            continue;
        }

        add_to_history(input);

        char command[MAX_INPUT];
        strncpy(command, input, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';

        char* token = strtok(command, " ");
        if (!token) {
            continue;
        }

        if (strcmp(token, "start_network") == 0) {
            if (runtime_start_network_service(8080)) {
                printf("Network services started\n");
            } else {
                printf("Failed to start network services\n");
            }
        } else if (strcmp(token, "stop_network") == 0) {
            runtime_stop_network_services();
            printf("Network services stopped\n");
        } else if (strcmp(token, "list_endpoints") == 0) {
            list_endpoints();
        } else if (strcmp(token, "list_clients") == 0) {
            list_clients();
        } else if (strcmp(token, "list_states") == 0) {
            list_states();
        } else if (strcmp(token, "list_transitions") == 0) {
            list_transitions();
        } else if (strcmp(token, "history") == 0) {
            show_history();
        } else if (strcmp(token, "status") == 0) {
            show_status();
        } else {
            runtime_process_command(&g_runtime, input);
        }
    }
}
