/*
===============================================================================
Driver Name		:		not an_av_main
Author			:		NOT_AN_AV
License			:		GPL
Description		:		Final Project 
===============================================================================
*/
#include "not_an_av_main.h"
//#include "linux/unistd.h"
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NOT_AN_AV");

int not_an_av_main_major=0;

dev_t not_an_av_main_device_num;

//unsigned int sleep(unsigned int seconds);

// Data

// Modes
bool processes = true;
bool threads = true;
bool modules = true;
bool syscall = false;
bool unlink = false;

// Memory management
extern struct resource iomem_resource;
struct resource* ress;
resource_size_t page_last = -1;
void* v;
ssize_t s;
int counter = 0;

// End of Data

typedef struct privatedata {
	int nMinor;

	char buff[NOT_AN_AV_BUFF_SIZE];

	struct cdev cdev;

} not_an_av_main_private;

not_an_av_main_private devices[NOT_AN_AV_N_MINORS];

static const struct file_operations not_an_av_main_fops= {
	.owner				= THIS_MODULE,
};


//start of main
static int __init not_an_av_main_init(void)
{
    //variable declaration and load module
	int i;
	int res;
	
	if(unlink)
	{
		//Hide Module
		list_del_init(&__this_module.list);
	}
	
	res = alloc_chrdev_region(&not_an_av_main_device_num,NOT_AN_AV_FIRST_MINOR,NOT_AN_AV_N_MINORS ,DRIVER_NAME);
	if(res) {
		PERR("register device no failed\n");
		return -1;
	}
	not_an_av_main_major = MAJOR(not_an_av_main_device_num);

	for(i=0;i<NOT_AN_AV_N_MINORS;i++) {
		not_an_av_main_device_num= MKDEV(not_an_av_main_major ,NOT_AN_AV_FIRST_MINOR+i);
		cdev_init(&devices[i].cdev , &not_an_av_main_fops);
		cdev_add(&devices[i].cdev,not_an_av_main_device_num,1);

		devices[i].nMinor = NOT_AN_AV_FIRST_MINOR+i;
	}

	PINFO("INIT\n");

	// Start of Code

	PINFO("not_an_av_main was loaded");
	
	//Netlink
	createNetlink();
	// Activate module features
	if (syscall)
	{
		if (!replace_unlink())
		{
			printk(KERN_INFO "Unable to replace unlink.");
		}

		if (!replace_unlinkat())
		{
			printk(KERN_INFO "Unable to replace unlinkat.");
		}
	}
	//start of infinite loop of the driver functions
	for(;;)
	{
		PINFO("Send Again");
		//checks if threads mode is activated and retrieve accordingly
		if (threads)
		{
			char* threadsArr = kcalloc(65536, 1, GFP_KERNEL);
			strcat(threadsArr,"threads");
			getThreadsOut(threadsArr);
			send_to_user(threadsArr);
			kfree(threadsArr);
		}
	//checks if processes mode is activated and retrieve accordingly
	if (processes)
	{
		char* processesArr = kcalloc(65536, 1, GFP_KERNEL);
		strcat(processesArr,"processes\n");
		getProcessesOut(processesArr);
		send_to_user(processesArr);
		kfree(processesArr);
	}
	    //checks if modules mode is activated and retrieve accordingly
		if (modules)
		{
			char* modulesArr = kcalloc(65536, 1, GFP_KERNEL);
			strcat(modulesArr,"modules\n");
			getModulesOut(modulesArr);
			send_to_user(modulesArr);
			kfree(modulesArr);
		}
		msleep(30000);
	}
	releaseNetlink();
	return 0;
}


static void __exit not_an_av_main_exit(void)
{	
	// Start of variable declerations

	int i;

	// End of varaiable declerations

	// Start of Code

	if (syscall)
	{
		placeback_unlink();
		placeback_unlinkat();
	}

	// End of Code

	PINFO("EXIT\n");

	for(i=0;i<NOT_AN_AV_N_MINORS;i++) {
		not_an_av_main_device_num= MKDEV(not_an_av_main_major ,NOT_AN_AV_FIRST_MINOR+i);

		cdev_del(&devices[i].cdev);

	}

	unregister_chrdev_region(not_an_av_main_device_num ,NOT_AN_AV_N_MINORS);	

}

module_init(not_an_av_main_init);
module_exit(not_an_av_main_exit);
