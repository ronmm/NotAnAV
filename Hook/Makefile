obj-m := lkh.o
lkh-objs += module.o
lkh-objs += sys_hook.o
lkh-objs += hooks.o
lkh-objs += netlink.o
lkh-objs += user2kernel.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
