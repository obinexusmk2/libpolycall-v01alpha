#ifndef POLYCALL_RUNTIME_H
#define POLYCALL_RUNTIME_H

#include "network.h"
#include "polycall.h"
#include "polycall_state_machine.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_INPUT 256
#define MAX_PORTS 64
#define MAX_PROGRAMS 8
#define HISTORY_SIZE 10

#define MAX_SERVICES 16
#define DEFAULT_PORT_BASE 3000
#define CONFIG_FILENAME ".polycallrc"

typedef struct {
    uint16_t host_port;
    uint16_t container_port;
    bool is_active;
} PortMapping;

typedef struct {
    PortMapping mappings[MAX_PORTS];
    size_t count;
} PortMappingArray;

typedef struct {
    NetworkProgram* programs[MAX_PROGRAMS];
    size_t program_count;
    polycall_context_t pc_ctx;
    PolyCall_StateMachine* state_machine;
    char command_history[HISTORY_SIZE][MAX_INPUT];
    int history_count;
    PolyCall_StateSnapshot snapshots[POLYCALL_MAX_STATES];
    bool has_snapshot[POLYCALL_MAX_STATES];
    PortMappingArray port_mappings;
#ifdef _WIN32
    bool wsa_initialized;
#endif
    bool running;
} PPI_Runtime;

typedef struct {
    char base_path[256];
    bool is_initialized;
} DirectoryControl;

extern PPI_Runtime g_runtime;

bool runtime_initialize(void);
void runtime_cleanup(void);
void runtime_register_signal_handlers(void);
void runtime_cleanup_and_exit(void);

void runtime_run_interactive(void);
bool runtime_run_non_interactive(const char* config_file);

bool runtime_start_network_service(uint16_t port);
void runtime_stop_network_services(void);
bool runtime_add_port_mapping(const char* service_name, uint16_t host_port, uint16_t container_port);

bool directory_init_control(const char* base_path);
void directory_cleanup_control(void);
bool directory_discover_and_init_services(void);
bool directory_process_config_file(const char* filename);
bool polycall_init_with_directory(const char* base_path, const char* config_file);

#endif
