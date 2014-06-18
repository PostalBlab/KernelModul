ifneq ($(KERNELRELEASE),)
obj-m	:= fluxi.o 
fluxi-objs	:= mod_test.o

else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif
