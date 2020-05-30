#include <linux/module.h>
#include <linux/sched.h>
#include "hooks.h"
#include "sys_hook.h"
#include "netlink.h"

extern struct sys_hook *lkh_sys_hook;

asmlinkage int
mkdir_hook(const char *path, int mode)
{
    printk(KERN_ALERT "DEBUG: MKDir Hook\n");
    
    sys_mkdir_t sys_mkdir;
    
    sys_mkdir = (sys_mkdir_t)sys_hook_get_orig64(lkh_sys_hook, __NR_mkdir);

    send_msg_from_kernel("MKDIR");

    return sys_mkdir(path, mode);
}

asmlinkage int
execve_hook(const char *pathname, char *const argv[], char *const envp[])
{
    printk(KERN_ALERT "DEBUG: ExecVE Hook\n");
    printk(KERN_ALERT "DEBUG: ExecVE params: pathname: %s, argv: %s, envp: %s \n", pathname, argv, envp); // TODO: Print the argument with a loop or something, right now it's all wrong
    printk(KERN_ALERT "DEBUG: ExecVE pid: %d\n", (int) task_pid_nr(current));

    send_msg_from_kernel("EXECVE");
    
    sys_execve_t sys_execve;
    
    sys_execve = (sys_execve_t)sys_hook_get_orig64(lkh_sys_hook, __NR_execve);

    return sys_execve(pathname, argv, envp);
}

asmlinkage int
connect_hook(int sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
    printk(KERN_ALERT "DEBUG: Connect Hook\n");
    printk(KERN_ALERT "DEBUG: Connect params: sockfd: %d, sockaddr: %s, addrlen: %d \n", sockfd, addr, addrlen); // TODO: Print the argument with a loop or something, right now it's all wrong
    printk(KERN_ALERT "DEBUG: Connect pid: %d\n", (int) task_pid_nr(current));

    sys_connect_t sys_connect;

    sys_connect = (sys_connect_t)sys_hook_get_orig64(lkh_sys_hook, __NR_connect);

    send_msg_from_kernel("CONNECT");

    return sys_connect(sockfd, addr, addrlen);
}