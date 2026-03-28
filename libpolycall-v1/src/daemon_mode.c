#include "daemon_mode.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if POLYCALL_HAS_POSIX_DAEMON
#include <fcntl.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int g_pid_fd = -1;
static char g_pid_path[512] = {0};

static bool redirect_streams(const char* log_file) {
    int stdin_fd = open("/dev/null", O_RDONLY);
    if (stdin_fd < 0 || dup2(stdin_fd, STDIN_FILENO) < 0) {
        if (stdin_fd >= 0) {
            close(stdin_fd);
        }
        return false;
    }
    close(stdin_fd);

    const char* target = log_file ? log_file : "/dev/null";
    int flags = O_WRONLY | O_CREAT | O_APPEND;
    int out_fd = open(target, flags, 0644);
    if (out_fd < 0) {
        return false;
    }

    if (dup2(out_fd, STDOUT_FILENO) < 0 || dup2(out_fd, STDERR_FILENO) < 0) {
        close(out_fd);
        return false;
    }

    if (out_fd > STDERR_FILENO) {
        close(out_fd);
    }

    return true;
}

bool daemonize_process(const PolyCallDaemonOptions* options) {
    if (!options || !options->detach) {
        return true;
    }

    pid_t pid = fork();
    if (pid < 0) {
        return false;
    }
    if (pid > 0) {
        _exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        return false;
    }

    pid = fork();
    if (pid < 0) {
        return false;
    }
    if (pid > 0) {
        _exit(EXIT_SUCCESS);
    }

    umask(027);
    if (chdir("/") != 0) {
        return false;
    }

    if (!redirect_streams(options->log_file)) {
        return false;
    }

    return true;
}

bool daemon_pidfile_create(const char* pid_file) {
    if (!pid_file || pid_file[0] == '\0') {
        return true;
    }

    int fd = open(pid_file, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        fprintf(stderr, "Failed to open pid file %s: %s\n", pid_file, strerror(errno));
        return false;
    }

    if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
        fprintf(stderr, "Failed to lock pid file %s: %s\n", pid_file, strerror(errno));
        close(fd);
        return false;
    }

    if (ftruncate(fd, 0) != 0) {
        fprintf(stderr, "Failed to truncate pid file %s: %s\n", pid_file, strerror(errno));
        close(fd);
        return false;
    }

    char pid_buf[32];
    int len = snprintf(pid_buf, sizeof(pid_buf), "%ld\n", (long)getpid());
    if (write(fd, pid_buf, (size_t)len) != len) {
        fprintf(stderr, "Failed to write pid file %s: %s\n", pid_file, strerror(errno));
        close(fd);
        return false;
    }

    g_pid_fd = fd;
    strncpy(g_pid_path, pid_file, sizeof(g_pid_path) - 1);
    g_pid_path[sizeof(g_pid_path) - 1] = '\0';
    return true;
}

void daemon_pidfile_remove(void) {
    if (g_pid_fd >= 0) {
        close(g_pid_fd);
        g_pid_fd = -1;
    }

    if (g_pid_path[0] != '\0') {
        unlink(g_pid_path);
        g_pid_path[0] = '\0';
    }
}

#else

bool daemonize_process(const PolyCallDaemonOptions* options) {
    (void)options;
    return true;
}

bool daemon_pidfile_create(const char* pid_file) {
    (void)pid_file;
    return true;
}

void daemon_pidfile_remove(void) {}

#endif
