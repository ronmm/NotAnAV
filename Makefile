obj-m		:= not_an_av.o

not_an_av-objs := not_an_av_main.o not_an_av_syscalls.o not_an_av_process.o not_an_av_modules.o netlink.o

KERN_SRC	:= /lib/modules/$(shell uname -r)/build/
KVER ?= $(shell uname -r)
PWD			:= $(shell pwd)

modules:
	make -C $(KERN_SRC) M=$(PWD) modules

install:
	make -C $(KERN_SRC) M=$(PWD) modules_install
	depmod -a

clean:
	make -C $(KERN_SRC) M=$(PWD) clean
