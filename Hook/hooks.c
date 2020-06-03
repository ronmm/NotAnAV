#include <linux/module.h>
#include <linux/sched.h>
#include "hooks.h"
#include "sys_hook.h"
#include "netlink.h"

extern struct sys_hook *lkh_sys_hook;

asmlinkage int
mkdir_hook(const char __user *path, int mode)
{
    printk(KERN_ALERT "DEBUG: MKDir Hook\n");
    
    sys_mkdir_t sys_mkdir;
    
    sys_mkdir = (sys_mkdir_t)sys_hook_get_orig64(lkh_sys_hook, __NR_mkdir);

    send_msg_from_kernel("MKDIR");

    return sys_mkdir(path, mode);
}

asmlinkage int
execve_hook(const char __user *pathname, char __user *const argv[], char __user *const envp[])
{
	
	printk(KERN_INFO "DEBUG: entered execve hook")
	
	sys_execve_t sys_execve;
	
    // Get the path name using strncpy_from_user
    char buf[256] = {0};
    if (strncpy_from_user(buf, pathname, sizeof(buf)) < 0)
        goto print_error;
	
	/*
	 * TODO: collect the content of argv and envp
	 * The following code caused crash:
	 *
	 * const char __user * ptr1;
	 * ptr1 = argv[0];
	 * strncpy_from_user(buf, ptr1, sizeof(buf)); 
	 *
	 */
    
    send_msg_from_kernel("EXECVE"); // TODO - send real data
   
    // Code to run the actual execve
    orig_execve:   
    sys_execve_t sys_execve;
    
    sys_execve = (sys_execve_t)sys_hook_get_orig64(lkh_sys_hook, __NR_execve);

    return sys_execve(pathname, argv, envp);
	
	// In case of error getting the relevant data - print error and then execute original execve
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
        snprintf(msg, 256, "CONNECT: address=%lx port=%hu pid=%d\n", htonl(sock_details.sin_addr.s_addr), sock_details.sin_port, (int)task_pid_nr(current));
        send_msg_from_kernel(msg);
    } else {
        printk(KERN_ALERT "CONNECT: unable to retrieve connection details from user space\n");
    }

    // Run the actual connect code
    sys_connect_t sys_connect;
    sys_connect = (sys_connect_t)sys_hook_get_orig64(lkh_sys_hook, __NR_connect);
    return sys_connect(sockfd, addr, addrlen);
}