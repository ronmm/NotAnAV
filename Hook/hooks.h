#pragma once

//#include <sys/socket.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include "types.h"
#include "netlink.h"
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/icmp.h>
#include <linux/ip.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <net/sock.h>
#include <net/net_namespace.h>

typedef asmlinkage int (*sys_execve_t)(const char *, char *const [], char *const []);
typedef asmlinkage int (*sys_connect_t)(int, const struct sockaddr *, unsigned int);
typedef asmlinkage int (*sys_chmod_t)(const char *, mode_t);
typedef asmlinkage int (*sys_fchmodat_t)(int, const char *, mode_t, int);
//typedef asmlinkage int (*sys_ptrace_t)(enum __ptrace_request, pid_t, void, void);
//typedef asmlinkage int (*sys_clone_t)(int, void *, int, void *, ...);

unsigned int watch_icmp(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);

asmlinkage int
execve_hook(const char *, char *const [], char *const []);

asmlinkage int
connect_hook(int sockfd, const struct sockaddr *addr, unsigned int addrlen);

asmlinkage int
chmod_hook(const char *path, mode_t mode);

asmlinkage int
fchmodat_hook(int dirfd, const char *pathname, mode_t mode, int flags);

//asmlinkage int
//ptrace_hook(enum __ptrace_request request, pid_t pid, void *addr, void *data);

//asmlinkage int
//clone_hook(int (*__fn) (void *__arg), void *__child_stack, int __flags, void *__arg, ...);
