#ifndef LINC_XSYS_H
#define LINC_XSYS_H

#include <linc.h>

#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <termios.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <signal.h>

#include "common.h"

/* 34 TOTAL */
#define xopen(...)            linc_xsys_open(LINC_SITE, __VA_ARGS__)
#define xdup2(...)            linc_xsys_dup2(LINC_SITE, __VA_ARGS__)
#define xclose(...)           linc_xsys_close(LINC_SITE, __VA_ARGS__)
#define xmalloc(...)          linc_xsys_malloc(LINC_SITE, __VA_ARGS__)
#define xcalloc(...)          linc_xsys_calloc(LINC_SITE, __VA_ARGS__)
#define xrealloc(...)         linc_xsys_realloc(LINC_SITE, __VA_ARGS__)
#define xsigaction(...)       linc_xsys_sigaction(LINC_SITE, __VA_ARGS__)
#define xsigemptyset(...)     linc_xsys_sigemptyset(LINC_SITE, __VA_ARGS__)
#define xsigaddset(...)       linc_xsys_sigaddset(LINC_SITE, __VA_ARGS__)
#define xsigdelset(...)       linc_xsys_sigdelset(LINC_SITE, __VA_ARGS__)
#define xsigprocmask(...)     linc_xsys_sigprocmask(LINC_SITE, __VA_ARGS__)
#define xsetpgid(...)         linc_xsys_setpgid(LINC_SITE, __VA_ARGS__)
#define xtcsetpgrp(...)       linc_xsys_tcsetpgrp(LINC_SITE, __VA_ARGS__)
#define xgetcwd(...)          linc_xsys_getcwd(LINC_SITE, __VA_ARGS__)
#define xkill(...)            linc_xsys_kill(LINC_SITE, __VA_ARGS__)
#define xatexit(...)          linc_xsys_atexit(LINC_SITE, __VA_ARGS__)
#define xpipe(...)            linc_xsys_pipe(LINC_SITE, __VA_ARGS__)
#define xpipe2(...)           linc_xsys_pipe2(LINC_SITE, __VA_ARGS__)
#define xfork()               linc_xsys_fork(LINC_SITE)
#define xtcgetattr(...)       linc_xsys_tcgetattr(LINC_SITE, __VA_ARGS__)
#define xtcsetattr(...)       linc_xsys_tcsetattr(LINC_SITE, __VA_ARGS__)
#define xforkpty(...)         linc_xsys_forkpty(LINC_SITE, __VA_ARGS__)
#define xpoll(...)            linc_xsys_poll(LINC_SITE, __VA_ARGS__)
#define xstrdup(...)          linc_xsys_strdup(LINC_SITE, __VA_ARGS__)
#define xstrndup(...)         linc_xsys_strndup(LINC_SITE, __VA_ARGS__)
#define xsetvbuf(...)         linc_xsys_setvbuf(LINC_SITE, __VA_ARGS__)
#define xopendir(...)         linc_xsys_opendir(LINC_SITE, __VA_ARGS__)
#define xclosedir(...)        linc_xsys_closedir(LINC_SITE, __VA_ARGS__)
#define xmkdir(...)           linc_xsys_mkdir(LINC_SITE, __VA_ARGS__)
#define xrmdir(...)           linc_xsys_rmdir(LINC_SITE, __VA_ARGS__)
#define xunlink(...)          linc_xsys_unlink(LINC_SITE, __VA_ARGS__)
#define xchdir(...)           linc_xsys_chdir(LINC_SITE, __VA_ARGS__)
#define xtimerfd_create(...)  linc_xsys_timerfd_create(LINC_SITE, __VA_ARGS__)
#define xtimerfd_settime(...) linc_xsys_timerfd_settime(LINC_SITE, __VA_ARGS__)

int linc_xsys_open(linc_site site, const char *pathname, int flags, ...);
int linc_xsys_close(linc_site site, int fd);
int linc_xsys_dup2(linc_site site, int oldfd, int newfd);
char *linc_xsys_getcwd(linc_site site, char *buf, size_t size);
DIR *linc_xsys_opendir(linc_site site, const char *name);
int linc_xsys_closedir(linc_site site, DIR *dirp);
int linc_xsys_mkdir(linc_site site, const char *pathname, mode_t mode);
int linc_xsys_rmdir(linc_site site, const char *pathname);
int linc_xsys_unlink(linc_site site, const char *pathname);
int linc_xsys_chdir(linc_site site, const char *pathname);

int linc_xsys_pipe(linc_site site, int pipefd[2]);
int linc_xsys_pipe2(linc_site site, int pipefd[2], int flags);
int linc_xsys_kill(linc_site site, pid_t pid, int sig);
int linc_xsys_fork(linc_site site);
int linc_xsys_forkpty(linc_site site, int *amaster, char *name,
        const struct termios *tio, const struct winsize *winp);
int linc_xsys_setpgid(linc_site site, pid_t pid, pid_t pgid);
int linc_xsys_poll(linc_site site, struct pollfd *fds, nfds_t nfds,
        int timeout);
int linc_xsys_timerfd_create(linc_site site, int clockid, int flags);
int linc_xsys_timerfd_settime(linc_site site, int fd, int flags,
        const struct itimerspec *new_value, struct itimerspec *old_value);

void *linc_xsys_malloc(linc_site site, size_t size);
void *linc_xsys_calloc(linc_site site, size_t nmemb, size_t size);
void *linc_xsys_realloc(linc_site site, void *ptr, size_t size);
char *linc_xsys_strdup(linc_site site, const char *s);
char *linc_xsys_strndup(linc_site site, const char *s, size_t n);

int linc_xsys_sigemptyset(linc_site site, sigset_t *set);
int linc_xsys_sigaddset(linc_site site, sigset_t *set, int signum);
int linc_xsys_sigdelset(linc_site site, sigset_t *set, int signum);
int linc_xsys_sigprocmask(linc_site site, int how, const sigset_t *set,
        sigset_t *oldset);
int linc_xsys_sigaction(linc_site site, int signum,
        const struct sigaction *act, struct sigaction *oldact);

int linc_xsys_tcsetpgrp(linc_site site, int fd, pid_t pgrp);
int linc_xsys_tcgetattr(linc_site site, int fd, struct termios *tio);
int linc_xsys_tcsetattr(linc_site site, int fd, int optional_actions,
        const struct termios *termios_p);

int linc_xsys_atexit(linc_site site, void (*function)(void));
int linc_xsys_setvbuf(linc_site site, FILE *stream, char *buf, int mode,
        size_t size);

#endif
