#ifndef POLYCALL_DAEMON_H
#define POLYCALL_DAEMON_H

#include <stdbool.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    mode_t umask_value;
    const char* working_directory;
    const char* log_file;
} polycall_daemon_options_t;

bool polycall_daemonize(const polycall_daemon_options_t* options);
bool polycall_pidfile_acquire(const char* pid_file);
void polycall_pidfile_release(void);

#ifdef __cplusplus
}
#endif

#endif /* POLYCALL_DAEMON_H */
