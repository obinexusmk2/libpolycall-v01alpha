#include "daemon.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)

polycall_daemon_result_t polycall_daemonize(const char* pid_file_path) {
    (void)pid_file_path;
    return POLYCALL_DAEMON_UNSUPPORTED;
}

void polycall_daemon_cleanup(void) {
}

#else

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static char g_pid_file_path[PATH_MAX];
static bool g_pid_file_is_active = false;

static bool write_pid_file(const char* pid_file_path) {
    if (!pid_file_path || pid_file_path[0] == '\0') {
        return true;
    }

    FILE* fp = fopen(pid_file_path, "w");
    if (!fp) {
        return false;
    }

    if (fprintf(fp, "%ld\n", (long)getpid()) < 0) {
        fclose(fp);
        return false;
    }

    fclose(fp);
    strncpy(g_pid_file_path, pid_file_path, sizeof(g_pid_file_path) - 1);
    g_pid_file_path[sizeof(g_pid_file_path) - 1] = '\0';
    g_pid_file_is_active = true;
    return true;
}

static int redirect_stdio_to_devnull(void) {
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) < 0 ||
        dup2(fd, STDOUT_FILENO) < 0 ||
        dup2(fd, STDERR_FILENO) < 0) {
        close(fd);
        return -1;
    }

    if (fd > STDERR_FILENO) {
        close(fd);
    }

    return 0;
}

polycall_daemon_result_t polycall_daemonize(const char* pid_file_path) {
    int ready_pipe[2] = {-1, -1};
    if (pipe(ready_pipe) != 0) {
        return POLYCALL_DAEMON_ERROR;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(ready_pipe[0]);
        close(ready_pipe[1]);
        return POLYCALL_DAEMON_ERROR;
    }

    if (pid > 0) {
        char ready = '0';
        ssize_t bytes_read;

        close(ready_pipe[1]);
        bytes_read = read(ready_pipe[0], &ready, sizeof(ready));
        close(ready_pipe[0]);

        if (bytes_read == 1 && ready == '1') {
            return POLYCALL_DAEMON_PARENT_EXIT;
        }

        return POLYCALL_DAEMON_ERROR;
    }

    close(ready_pipe[0]);

    if (setsid() < 0) {
        (void)write(ready_pipe[1], "0", 1);
        close(ready_pipe[1]);
        _exit(1);
    }

    pid = fork();
    if (pid < 0) {
        (void)write(ready_pipe[1], "0", 1);
        close(ready_pipe[1]);
        _exit(1);
    }

    if (pid > 0) {
        _exit(0);
    }

    umask(0);
    (void)chdir("/");

    if (redirect_stdio_to_devnull() != 0) {
        (void)write(ready_pipe[1], "0", 1);
        close(ready_pipe[1]);
        return POLYCALL_DAEMON_ERROR;
    }

    if (!write_pid_file(pid_file_path)) {
        (void)write(ready_pipe[1], "0", 1);
        close(ready_pipe[1]);
        return POLYCALL_DAEMON_ERROR;
    }

    (void)write(ready_pipe[1], "1", 1);
    close(ready_pipe[1]);

    return POLYCALL_DAEMON_OK_CHILD;
}

void polycall_daemon_cleanup(void) {
    if (g_pid_file_is_active) {
        (void)unlink(g_pid_file_path);
        g_pid_file_is_active = false;
    }
}

#endif
