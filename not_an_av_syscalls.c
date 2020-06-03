#include "not_an_av_syscalls.h"

// Syscall management
// Pointer to syscall table
unsigned long **sys_call_table;
unsigned long original_cr0;

// Pointer to original syscall
asmlinkage int (*original_call_unlink) (const char *);
asmlinkage int (*original_call_unlinkat) (int dirfd, const char *pathname, int flags);

static int uid;
module_param(uid, int, 0644);

// Replacing the unlink syscall
bool replace_unlink(void)
{
    if(!(sys_call_table = aquire_sys_call_table()))
        return false;

    original_cr0 = read_cr0();

    write_cr0(original_cr0 & ~0x00010000);

    /* keep track of the original unlink function */
    original_call_unlink = (void*)sys_call_table[__NR_unlink];

    /* use our unlink function instead */
    sys_call_table[__NR_unlink] = (unsigned long *)our_sys_unlink;

    write_cr0(original_cr0);

    printk(KERN_INFO "Replaced unlink syscall\n");

    return true;
}

// Reverting syscall replacement
void placeback_unlink(void)
{
    if(!sys_call_table) {
    	printk(KERN_INFO "Failed to load sys_call_table");
        return;
    }

    /*
     * Return the system call back to normal
     */
    if (sys_call_table[__NR_unlink] != (unsigned long *)our_sys_unlink) {
        printk(KERN_ALERT "Somebody else also played with the ");
        printk(KERN_ALERT "unlink system call\n");
        printk(KERN_ALERT "The system may be left in ");
        printk(KERN_ALERT "an unstable state.\n");
    }

    write_cr0(original_cr0 & ~0x00010000);
    sys_call_table[__NR_unlink] = (unsigned long *)original_call_unlink;
    write_cr0(original_cr0);

    msleep(2000);
}

// Modified unlink syscall
asmlinkage int our_sys_unlink(const char* filename)
{
    if (strcmp(filename,"user.o") == 0)
    {
        printk("%s has tried to remove user.o!", current->comm);
        return -1;
    }

    /*
     * Call the original sys_unlink - otherwise, we lose
     * the ability to remove files
     */
    return original_call_unlink(filename);
}

bool replace_unlinkat(void)
{
    if(!(sys_call_table = aquire_sys_call_table()))
        return false;

    original_cr0 = read_cr0();

    write_cr0(original_cr0 & ~0x00010000);

    /* keep track of the original unlink function */
    original_call_unlinkat = (void*)sys_call_table[__NR_unlinkat];

    /* use our unlink function instead */
    sys_call_table[__NR_unlinkat] = (unsigned long *)our_sys_unlinkat;

    write_cr0(original_cr0);

    printk(KERN_INFO "Replaced unlinkat syscall\n");

    return true;
}

// Reverting syscall replacement
void placeback_unlinkat(void)
{
    if(!sys_call_table) {
    	printk(KERN_INFO "Failed to load sys_call_table");
        return;
    }

    /*
     * Return the system call back to normal
     */
    if (sys_call_table[__NR_unlinkat] != (unsigned long *)our_sys_unlinkat) {
        printk(KERN_ALERT "Somebody else also played with the ");
        printk(KERN_ALERT "unlinkat system call\n");
        printk(KERN_ALERT "The system may be left in ");
        printk(KERN_ALERT "an unstable state.\n");
    }

    write_cr0(original_cr0 & ~0x00010000);
    sys_call_table[__NR_unlinkat] = (unsigned long *)original_call_unlinkat;
    write_cr0(original_cr0);

    msleep(2000);
}

// Modified unlink syscall
asmlinkage int our_sys_unlinkat(int dirfd, const char *pathname, int flags)
{
    if (strcmp(pathname,"user.o") == 0)
    {
        printk("%s has tried to remove user.o!", current->comm);
        return -1;
    }

    /*
     * Call the original sys_unlink - otherwise, we lose
     * the ability to remove files
     */
    return original_call_unlinkat(dirfd, pathname, flags);
}

// Fetch syscall table
unsigned long **aquire_sys_call_table(void)
{
    unsigned long int offset = PAGE_OFFSET;
    unsigned long **sct;

    while (offset < ULLONG_MAX) {
        sct = (unsigned long **)offset;

        if (sct[__NR_close] == (unsigned long *) sys_close)
            return sct;

        offset += sizeof(void *);
    }

    printk(KERN_INFO "aquire_sys_call_table about to return NULL");

    return NULL;
}
