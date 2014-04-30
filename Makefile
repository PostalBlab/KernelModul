ifneq ($(KERNELRELEASE),)
obj-m	:= fluxator.o 
fluxator-objs	:= mod_test.o printm.o kmm.o

else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif
