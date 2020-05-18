#pragma once

//#include <sys/socket.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include "types.h"

typedef asmlinkage int (*sys_mkdir_t)(const char *, int);
typedef asmlinkage int (*sys_execve_t)(const char *, char *const [], char *const []);
typedef asmlinkage int (*sys_connect_t)(int, const struct sockaddr *, unsigned int);

asmlinkage int
mkdir_hook(const char *, int);

asmlinkage int
execve_hook(const char *, char *const [], char *const []);

asmlinkage int
connect_hook(int sockfd, const struct sockaddr *addr, unsigned int addrlen);
