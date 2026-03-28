#include "polycall_runtime.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static DirectoryControl g_directory = {0};

static bool validate_port_mapping(const PortMapping* mapping) {
    return mapping && mapping->host_port > 0 && mapping->container_port > 0;
}

static bool is_port_available(const PortMappingArray* mappings, uint16_t port) {
    for (size_t i = 0; i < mappings->count; i++) {
        if (mappings->mappings[i].is_active &&
            (mappings->mappings[i].host_port == port || mappings->mappings[i].container_port == port)) {
            return false;
        }
    }
    return true;
}

bool directory_init_control(const char* base_path) {
    struct stat st;

    if (!base_path || stat(base_path, &st) == -1) {
        fprintf(stderr, "Invalid base path: %s\n", strerror(errno));
        return false;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Not a directory: %s\n", base_path);
        return false;
    }

    strncpy(g_directory.base_path, base_path, sizeof(g_directory.base_path) - 1);
    g_directory.base_path[sizeof(g_directory.base_path) - 1] = '\0';
    g_directory.is_initialized = true;
    return true;
}

void directory_cleanup_control(void) {
    g_directory.is_initialized = false;
}

bool runtime_add_port_mapping(const char* service_name, uint16_t host_port, uint16_t container_port) {
    char path_buf[512];
    struct stat st;

    PortMapping temp_mapping = {
        .host_port = host_port,
        .container_port = container_port,
        .is_active = true,
    };

    if (!service_name || !validate_port_mapping(&temp_mapping)) {
        return false;
    }

    if (!g_directory.is_initialized) {
        fprintf(stderr, "Directory control is not initialized\n");
        return false;
    }

    if (g_runtime.port_mappings.count >= MAX_PORTS) {
        fprintf(stderr, "Maximum number of port mappings reached\n");
        return false;
    }

    if (!is_port_available(&g_runtime.port_mappings, host_port) ||
        !is_port_available(&g_runtime.port_mappings, container_port)) {
        fprintf(stderr, "Port conflict detected\n");
        return false;
    }

    snprintf(path_buf, sizeof(path_buf), "%s/%s", g_directory.base_path, service_name);
    if (stat(path_buf, &st) == -1 && mkdir(path_buf, 0755) == -1) {
        fprintf(stderr, "Failed to create service directory: %s\n", strerror(errno));
        return false;
    }

    if (g_runtime.program_count >= MAX_PROGRAMS) {
        fprintf(stderr, "Maximum network program count reached\n");
        return false;
    }

    PortMapping* mapping = &g_runtime.port_mappings.mappings[g_runtime.port_mappings.count++];
    mapping->host_port = host_port;
    mapping->container_port = container_port;
    mapping->is_active = true;

    if (!runtime_start_network_service(container_port)) {
        g_runtime.port_mappings.count--;
        return false;
    }

    printf("Mapped port %d to %d for service %s\n", host_port, container_port, service_name);
    return true;
}

bool directory_discover_and_init_services(void) {
    if (!g_directory.is_initialized) {
        return false;
    }

    DIR* current_dir = opendir(g_directory.base_path);
    if (!current_dir) {
        return false;
    }

    struct dirent* dir_entry = NULL;
    char config_path[512];
    char line[256];
    size_t services_found = 0;

    while ((dir_entry = readdir(current_dir)) != NULL) {
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
            continue;
        }

        size_t path_len = snprintf(config_path,
                                   sizeof(config_path),
                                   "%s/%s/%s",
                                   g_directory.base_path,
                                   dir_entry->d_name,
                                   CONFIG_FILENAME);

        if (path_len >= sizeof(config_path)) {
            fprintf(stderr, "Path too long for service: %s\n", dir_entry->d_name);
            continue;
        }

        FILE* config = fopen(config_path, "r");
        if (!config) {
            continue;
        }

        uint16_t host_port = DEFAULT_PORT_BASE + (uint16_t)services_found;
        uint16_t container_port = 8080 + (uint16_t)services_found;

        while (fgets(line, sizeof(line), config)) {
            if (strncmp(line, "port=", 5) == 0) {
                sscanf(line + 5, "%hu:%hu", &host_port, &container_port);
                break;
            }
        }

        if (runtime_add_port_mapping(dir_entry->d_name, host_port, container_port)) {
            services_found++;
        }

        fclose(config);
    }

    closedir(current_dir);
    return services_found > 0;
}

bool directory_process_config_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        return false;
    }

    char line[256];
    char service[32];
    uint16_t host_port;
    uint16_t container_port;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        if (sscanf(line, "server %31s %hu:%hu", service, &host_port, &container_port) == 3) {
            runtime_add_port_mapping(service, host_port, container_port);
        }
    }

    fclose(fp);
    return true;
}

bool polycall_init_with_directory(const char* base_path, const char* config_file) {
    if (!directory_init_control(base_path)) {
        return false;
    }

    if (config_file && !directory_process_config_file(config_file)) {
        fprintf(stderr, "Warning: Failed to process config file\n");
    }

    if (!directory_discover_and_init_services()) {
        fprintf(stderr, "Warning: No services discovered\n");
    }

    return true;
}
