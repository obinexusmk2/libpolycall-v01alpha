#ifndef POLYCALL_DAEMON_MODE_H
#define POLYCALL_DAEMON_MODE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool detach;
    const char* pid_file;
    const char* log_file;
} PolyCallDaemonOptions;

#if defined(_WIN32)
#define POLYCALL_HAS_POSIX_DAEMON 0
#else
#define POLYCALL_HAS_POSIX_DAEMON 1
#endif

bool daemonize_process(const PolyCallDaemonOptions* options);
bool daemon_pidfile_create(const char* pid_file);
void daemon_pidfile_remove(void);

#ifdef __cplusplus
}
#endif

#endif
