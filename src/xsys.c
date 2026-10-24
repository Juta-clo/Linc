#include <linc/xsys.h>
#include <linc.h>

#include <fcntl.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>
#include <pty.h>
#include <poll.h>
#include <sys/timerfd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#include <dirent.h>

#include "internal.h"

int linc_xsys_open(linc_site site, const char *pathname, int flags, ...) {
    va_list va;
    mode_t mode = 0;

    if (flags & O_CREAT || (flags & O_TMPFILE) == O_TMPFILE) {
        va_start(va, flags);
        mode = va_arg(va, mode_t);
        va_end(va);
    }

    int rv = open(pathname, flags, mode);
    if (rv == -1)
        linc_fatal("open", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_dup2(linc_site site, int oldfd, int newfd) {
    int rv = dup2(oldfd, newfd);
    if (rv == -1)
        linc_fatal("dup2", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_close(linc_site site, int fd) {
    int rv = close(fd);
    if (rv == -1)
        linc_fatal("close", site, LINC_FATAL_SYS);

    return rv;
}

void *linc_xsys_malloc(linc_site site, size_t size) {
    if (size == 0)
        return NULL;

    void *rv = malloc(size);
    if (!rv)
        linc_fatal("malloc", site, LINC_FATAL_OOM);

    return rv;
}

void *linc_xsys_calloc(linc_site site, size_t n, size_t size) {
    if (n == 0 || size == 0)
        return NULL;

    void *rv = calloc(n, size);
    if (!rv)
        linc_fatal("calloc", site, LINC_FATAL_OOM);

    return rv;
}

void *linc_xsys_realloc(linc_site site, void *ptr, size_t size) {
    if (size == 0)
        return NULL;

    void *rv = realloc(ptr, size);
    if (!rv)
        linc_fatal("realloc", site, LINC_FATAL_OOM);

    return rv;
}

int linc_xsys_sigaction(linc_site site, int signum,
        const struct sigaction *act, struct sigaction *oldact) {
    int rv = sigaction(signum, act, oldact);
    if (rv == -1)
        linc_fatal("sigaction", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_sigemptyset(linc_site site, sigset_t *set) {
    int rv = sigemptyset(set);
    if (rv == -1)
        linc_fatal("sigemptyset", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_sigaddset(linc_site site, sigset_t *set, int signum) {
    int rv = sigaddset(set, signum);
    if (rv == -1)
        linc_fatal("sigaddset", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_sigdelset(linc_site site, sigset_t *set, int signum) {
    int rv = sigdelset(set, signum);
    if (rv == -1)
        linc_fatal("sigdelset", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_sigprocmask(linc_site site, int how, const sigset_t *set,
        sigset_t *oldset) {
    int rv = sigprocmask(how, set, oldset);
    if (rv == -1)
        linc_fatal("sigprocmask", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_setpgid(linc_site site, pid_t pid, pid_t pgid) {
    int rv = setpgid(pid, pgid);
    if (rv == -1)
        linc_fatal("setpgid", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_tcsetpgrp(linc_site site, int fd, pid_t pgrp) {
    int rv = tcsetpgrp(fd, pgrp);
    if (rv == -1)
        linc_fatal("tcsetpgrp", site, LINC_FATAL_SYS);

    return rv;
}

char * linc_xsys_getcwd(linc_site site, char *buf, size_t size) {
    char *rv = getcwd(buf, size);
    if (!rv)
        linc_fatal("getcwd", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_kill(linc_site site, pid_t pid, int sig) {
    int rv = kill(pid, sig);
    if (rv == -1)
        linc_fatal("kill", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_atexit(linc_site site, void (*function)(void)) {
    int rv = atexit(function);
    if (rv != 0)
        linc_fatal("atexit: failed", site, LINC_FATAL_LIB);

    return rv;
}

int linc_xsys_pipe(linc_site site, int pipefd[2]) {
    int rv = pipe(pipefd);
    if (rv == -1)
        linc_fatal("pipe", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_pipe2(linc_site site, int pipefd[2], int flags) {
    int rv = pipe2(pipefd, flags);
    if (rv == -1)
        linc_fatal("pipefd", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_fork(linc_site site) {
    int rv = fork();
    if (rv == -1)
        linc_fatal("fork", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_tcgetattr(linc_site site, int fd, struct termios *tio) {
    int rv = tcgetattr(fd, tio);
    if (rv == -1)
        linc_fatal("tcgetattr", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_tcsetattr(linc_site site, int fd, int optional_actions,
        const struct termios *termios_p)
{
    int rv = tcsetattr(fd, optional_actions, termios_p);
    if (rv == -1)
        linc_fatal("tcsetattr", site, LINC_FATAL_SYS);

    return rv;
}


int linc_xsys_forkpty(linc_site site, int *amaster, char *name,
        const struct termios *tio, const struct winsize *winp) {
    int rv = forkpty(amaster, name, tio, winp);
    if (rv == -1)
        linc_fatal("forkpty", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_poll(linc_site site, struct pollfd *fds, nfds_t nfds,
        int timeout) {
    int rv = poll(fds, nfds, timeout);
    if (rv == -1)
        linc_fatal("poll", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_timerfd_create(linc_site site, int clockid, int flags) {
    int rv = timerfd_create(clockid, flags);
    if (rv == -1)
        linc_fatal("timerfd_create", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_timerfd_settime(linc_site site, int fd, int flags,
        const struct itimerspec *new_value, struct itimerspec *old_value) {
    int rv = timerfd_settime(fd, flags, new_value, old_value);
    if (rv == -1)
        linc_fatal("timerfd_settime", site, LINC_FATAL_SYS);

    return rv;
}

char * linc_xsys_strdup(linc_site site, const char *s) {
    char *rv = strdup(s);
    if (!rv)
        linc_fatal("strdup", site, LINC_FATAL_SYS);

    return rv;
}

char * linc_xsys_strndup(linc_site site, const char *s, size_t n) {
    char *rv = strndup(s, n);
    if (!rv)
        linc_fatal("strndup", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_setvbuf(linc_site site, FILE *stream, char *buf,
        int mode, size_t size) {
    int rv = setvbuf(stream, buf, mode, size);
    if (rv != 0)
        linc_fatal("setvbuf: mode is invalid or the request cannot be honored",
                site, LINC_FATAL_LIB);

    return rv;
}

DIR * linc_xsys_opendir(linc_site site, const char *name) {
    DIR *rv = opendir(name);
    if (!rv)
        linc_fatal("opendir", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_closedir(linc_site site, DIR *dirp) {
    int rv = closedir(dirp);
    if (rv == -1)
        linc_fatal("closedir", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_mkdir(linc_site site, const char *pathname, mode_t mode) {
    int rv = mkdir(pathname, mode);
    if (rv == -1)
        linc_fatal("mkdir", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_rmdir(linc_site site, const char *pathname) {
    int rv = rmdir(pathname);
    if (rv == -1)
        linc_fatal("rmdir", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_unlink(linc_site site, const char *pathname) {
    int rv = unlink(pathname);
    if (rv == -1)
        linc_fatal("unlink", site, LINC_FATAL_SYS);

    return rv;
}

int linc_xsys_chdir(linc_site site, const char *pathname) {
    int rv = chdir(pathname);
    if (rv == -1)
        linc_fatal("chdir", site, LINC_FATAL_SYS);

    return rv;
}
