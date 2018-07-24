obj-m	:= my_module_1.o my_module_2.o

#KDIR	:= /lib/modules/$(shell uname -r)/build
KDIR	:= /work_kernel/linux

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$$PWD clean
