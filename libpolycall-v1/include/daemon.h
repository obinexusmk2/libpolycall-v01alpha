#ifndef POLYCALL_DAEMON_H
#define POLYCALL_DAEMON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    POLYCALL_DAEMON_OK_CHILD = 0,
    POLYCALL_DAEMON_PARENT_EXIT = 1,
    POLYCALL_DAEMON_ERROR = -1,
    POLYCALL_DAEMON_UNSUPPORTED = -2
} polycall_daemon_result_t;

polycall_daemon_result_t polycall_daemonize(const char* pid_file_path);
void polycall_daemon_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* POLYCALL_DAEMON_H */
