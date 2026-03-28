#include "daemon.h"

#ifndef _WIN32
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int g_pid_fd = -1;
static char g_pid_path[512] = {0};

static bool write_pid_to_fd(int fd, pid_t pid) {
    char pid_buf[32];
    int len = snprintf(pid_buf, sizeof(pid_buf), "%ld\n", (long)pid);

    if (len <= 0 || (size_t)len >= sizeof(pid_buf)) {
        return false;
    }

    if (ftruncate(fd, 0) == -1 || lseek(fd, 0, SEEK_SET) == -1) {
        return false;
    }

    if (write(fd, pid_buf, (size_t)len) != len) {
        return false;
    }

    return true;
}

bool polycall_daemonize(const polycall_daemon_options_t* options) {
    pid_t pid;
    const char* workdir = "/";
    mode_t daemon_umask = 027;
    const char* log_file = NULL;
    int null_fd = -1;
    int out_fd = -1;

    if (options != NULL) {
        if (options->working_directory != NULL) {
            workdir = options->working_directory;
        }
        daemon_umask = options->umask_value;
        log_file = options->log_file;
    }

    pid = fork();
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

    umask(daemon_umask);

    if (chdir(workdir) < 0) {
        return false;
    }

    null_fd = open("/dev/null", O_RDWR);
    if (null_fd < 0) {
        return false;
    }

    if (dup2(null_fd, STDIN_FILENO) < 0) {
        close(null_fd);
        return false;
    }

    if (log_file != NULL) {
        out_fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    } else {
        out_fd = dup(null_fd);
    }

    if (out_fd < 0) {
        close(null_fd);
        return false;
    }

    if (dup2(out_fd, STDOUT_FILENO) < 0 || dup2(out_fd, STDERR_FILENO) < 0) {
        close(out_fd);
        close(null_fd);
        return false;
    }

    if (out_fd != null_fd) {
        close(out_fd);
    }
    close(null_fd);

    return true;
}

bool polycall_pidfile_acquire(const char* pid_file) {
    int fd;
    char pid_buf[32] = {0};
    ssize_t bytes_read;
    long existing_pid;

    if (pid_file == NULL || *pid_file == '\0') {
        errno = EINVAL;
        return false;
    }

    fd = open(pid_file, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd < 0 && errno == EEXIST) {
        fd = open(pid_file, O_RDONLY);
        if (fd >= 0) {
            bytes_read = read(fd, pid_buf, sizeof(pid_buf) - 1);
            close(fd);

            if (bytes_read > 0) {
                existing_pid = strtol(pid_buf, NULL, 10);
                if (existing_pid > 1 && kill((pid_t)existing_pid, 0) == -1 && errno == ESRCH) {
                    unlink(pid_file);
                    fd = open(pid_file, O_RDWR | O_CREAT | O_EXCL, 0644);
                } else {
                    errno = EEXIST;
                    return false;
                }
            } else {
                errno = EEXIST;
                return false;
            }
        }
    }

    if (fd < 0) {
        return false;
    }

    if (!write_pid_to_fd(fd, getpid())) {
        close(fd);
        unlink(pid_file);
        return false;
    }

    g_pid_fd = fd;
    strncpy(g_pid_path, pid_file, sizeof(g_pid_path) - 1);
    g_pid_path[sizeof(g_pid_path) - 1] = '\0';

    return true;
}

void polycall_pidfile_release(void) {
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

bool polycall_daemonize(const polycall_daemon_options_t* options) {
    (void)options;
    return false;
}

bool polycall_pidfile_acquire(const char* pid_file) {
    (void)pid_file;
    return false;
}

void polycall_pidfile_release(void) {
}

#endif
