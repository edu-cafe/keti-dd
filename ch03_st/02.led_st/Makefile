obj-m	:= led_drv.o
APP	= led_app

#KDIR	:= /lib/modules/$(shell uname -r)/build
KDIR	:= /work_kernel/linux
CC = /opt/gnueabi/opt/ext-toolchain/bin/arm-linux-gnueabihf-gcc

#PWD	:= $(shell pwd)

all : module app

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$$PWD modules

app:
	$(CC) -o $(APP) $(APP).c

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	-rm $(APP)
