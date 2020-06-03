obj-m += module.o
KDIR ?= /
KVER ?= $(shell uname -r)

-include version.mk

all: dwarf 

dwarf: module.c
	$(MAKE) -C $(KDIR)/lib/modules/$(KVER)/build CONFIG_DEBUG_INFO=y M="$(shell pwd)" modules
	dwarfdump -di module.ko > module.dwarf
	$(MAKE) -C $(KDIR)/lib/modules/$(KVER)/build M="$(shell pwd)" clean

clean:
	$(MAKE) -C $(KDIR)/lib/modules/$(KVER)/build M="$(shell pwd)" clean
	rm -f module.dwarf
