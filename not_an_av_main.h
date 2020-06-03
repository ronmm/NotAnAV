/*
 *************************************************************************************

 * NOT_AN_AV module:
 * =================
 * 
 * To compile, run "make"
 * 
 * Main files:
 * ===========
 * not_an_av_main.c:
 *
 *        1) Inits the module and registers the device
 *        2) Enters an infinite loop and collects data every 30 seconds
 *        3) Each topic is being collected, and then sent to user mode using the function "send_to_user"
 * 
 * Data collection files:
 * ======================
 * 1) not_an_av_modules.c: extracts information about loaded kernel modules
 * 2) not_an_av_process.c: extracts information about processes and threads
 *
 * Helper files:
 * =============
 * not_an_av_syscalls.c:
 *      1) Replaces unlink and unlinkat syscalls during init and replaces them on module exit
 *      2) Monitors whether anyone attempted to kill the user-space process of the aplication
 *
 * Networking files:
 * ================
 * 1) netlink.c: sends the information from the kernel to user space
 * 2) user.c:    receives data from the kernel, adds more data and sends it to server

 **************************************************************************************
*/



#include <linux/sched.h>
#include <net/sock.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include "not_an_av_syscalls.h"
#include "not_an_av_process.h"
#include "not_an_av_modules.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/errno.h> 
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>  
#include <asm/page.h>  
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include "netlink.h"

#define DRIVER_NAME "not_an_av"
#define PDEBUG(fmt,args...) printk(KERN_DEBUG"%s:"fmt,DRIVER_NAME, ##args)
#define PERR(fmt,args...) printk(KERN_ERR"%s:"fmt,DRIVER_NAME,##args)
#define PINFO(fmt,args...) printk(KERN_INFO"%s:"fmt,DRIVER_NAME, ##args)

#define NOT_AN_AV_N_MINORS 1
#define NOT_AN_AV_FIRST_MINOR 0
#define NOT_AN_AV_BUFF_SIZE 1024

static int __init not_an_av_init(void);
static void __exit not_an_av_exit(void);
