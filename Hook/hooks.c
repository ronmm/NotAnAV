#include <linux/module.h>
#include <linux/sched.h>
#include "hooks.h"
#include "sys_hook.h"
#include "netlink.h"
#include <linux/inet.h>
#include "user2kernel.h"

extern struct sys_hook *lkh_sys_hook;

asmlinkage int
execve_hook(const char __user *pathname, char * const argv[], char *const envp[])
{
	sys_execve_t sys_execve;

	char argv_content[1024] = {0};
	if (conv_string_arr(argv_content, argv))
		goto print_error;

	char envp_content[1024] = {0};
	if (conv_string_arr(envp_content, envp))
		goto print_error;

	// Get the path name using strncpy_from_user
   	char l_pathname[256] = {0};
	const char __user * ptr1;
	if (strncpy_from_user(l_pathname, pathname, sizeof(l_pathname)) <= 0)
		goto print_error;

	char msg[4096] = {0};
	snprintf(msg, 1024, "EXECVE: \npathname=%s \nargv=%s \nenvp=%s \npid=%d \n", l_pathname, argv_content, envp_content, (int)task_pid_nr(current));

	send_msg_from_kernel(msg);
    
	// Execute the original code
	orig_execve:
    
	sys_execve = (sys_execve_t)sys_hook_get_orig64(lkh_sys_hook, __NR_execve);

	return sys_execve(pathname, argv, envp);

	// In case of error - print log and execute original sys call
	print_error:
	printk(KERN_INFO "DEBUG: error getting execve details\n");
	goto orig_execve;
}

asmlinkage int
connect_hook(int sockfd, const struct sockaddr __user * addr, unsigned int addrlen)
{
    // sockaddr and sockaddr_in are the same but organized differently
    // Therefore we will copy the sockaddr from user space into a sockaddr_in, in order to print the details
    struct sockaddr_in sock_details;
    char msg[256] = {0};

    // If copy from user was successful - send log to user space
    if(copy_from_user(&sock_details, addr, sizeof(struct sockaddr_in)) == 0)
    {
        snprintf(msg, 256, "CONNECT: \naddress=%lx \nport=%hu \npid=%d\n", htonl(sock_details.sin_addr.s_addr), sock_details.sin_port, (int)task_pid_nr(current));
        send_msg_from_kernel(msg);
    } else {
        printk(KERN_ALERT "CONNECT: unable to retrieve connection details from user space\n");
    }

    sys_connect_t sys_connect;

    sys_connect = (sys_connect_t)sys_hook_get_orig64(lkh_sys_hook, __NR_connect);

    return sys_connect(sockfd, addr, addrlen);
}

asmlinkage int
chmod_hook(const char *path, mode_t mode)
{
    char chmod_msg[20] = {0};
    snprintf(chmod_msg, 20, "CHMOD: pid=%d\n\0", (int)task_pid_nr(current));
    send_msg_from_kernel(chmod_msg);
    printk(KERN_INFO "%s\n", chmod_msg);

    sys_chmod_t sys_chmod;

    sys_chmod = (sys_chmod_t)sys_hook_get_orig64(lkh_sys_hook, __NR_chmod);

    return sys_chmod(path, mode);
}

asmlinkage int
fchmodat_hook(int dirfd, const char *pathname, mode_t mode, int flags)
{

    sys_fchmodat_t sys_fchmodat;
    char l_pathname[256] = {0};
    if (strncpy_from_user(l_pathname, pathname, sizeof(l_pathname)) <= 0)
        goto print_error;

    char msg[1024] = {0};
    snprintf(msg, 1024, "FCHMODAT: \npathname=%s \nmode=%3o \npid=%d",
        l_pathname,
        mode&0777,
        (int)task_pid_nr(current));
    
    send_msg_from_kernel(msg);

    orig_fchmodat:
    sys_fchmodat = (sys_fchmodat_t)sys_hook_get_orig64(lkh_sys_hook, __NR_fchmodat);
    return sys_fchmodat(dirfd, pathname, mode, flags);
    
    print_error:
    printk(KERN_INFO "DEBUG: error getting fchmodat details\n");
    goto orig_fchmodat;
    
}

//asmlinkage int
//ptrace_hook(enum __ptrace_request request, pid_t pid, void *addr, void *data)
//{
//    char msg[256] = {0};
//    snprintf(msg, 256, "PTRACE: pid=%d\n", (int)task_pid_nr(current));
//    send_msg_from_kernel(msg);
//
//    sys_ptrace_t sys_ptrace;
//
//    sys_ptrace = (sys_ptrace_t)sys_hook_get_orig64(lkh_sys_hook, __NR_ptrace);
//
//    return sys_clone(request, pid, addr, data);
//}

// asmlinkage int
//clone_hook(int (*__fn) (void *__arg), void *__child_stack, int __flags, void *__arg, ...)
//{
//    char msg[256] = {0};
//    snprintf(msg, 256, "CLONE: pid=%d\n", (int)task_pid_nr(current));
//    send_msg_from_kernel(msg);
//
//    sys_clone_t sys_clone;
//
//    sys_clone = (sys_clone_t)sys_hook_get_orig64(lkh_sys_hook, __NR_clone);
//
//    return sys_clone((*__fn) __arg, __child_stack, __flags, __arg, ...);
//}

unsigned int watch_icmp(void *priv,
                        struct sk_buff *skb,
                        const struct nf_hook_state *state)
{
    struct iphdr *ip_header;

    ip_header = ip_hdr(skb);
    if (!ip_header)
        return NF_ACCEPT;

    if ( ip_header->protocol != IPPROTO_ICMP )
        return NF_ACCEPT;

    printk(KERN_INFO "DEBUG: got ICMP!\n");

    send_msg_from_kernel("ICMP");

    return NF_ACCEPT;
}
