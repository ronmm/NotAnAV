#include <linux/sched.h>
#include <net/sock.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kdev_t.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/uaccess.h>

// Syscall replacement includes
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <asm/paravirt.h>
#include <linux/moduleparam.h>  /* which will have params */
#include <linux/unistd.h>       /* The list of system calls */
#include <linux/string.h>

// Non-generated includes
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/highmem.h>

unsigned long **aquire_sys_call_table(void);

bool replace_unlink(void);
void placeback_unlink(void);
asmlinkage int our_sys_unlink(const char* filename);

bool replace_unlinkat(void);
void placeback_unlinkat(void);
asmlinkage int our_sys_unlinkat(int dirfd, const char *pathname, int flags);
