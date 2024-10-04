MODULE_NAME=virt_net_if
PATH_SRC=$(shell pwd)
KERNEL=$(shell uname -r)

obj-m += $(MODULE_NAME).o

all: build install

build:
	/usr/bin/make -C /lib/modules/$(KERNEL)/build M=$(PATH_SRC) modules

clean:
	/usr/bin/make -C /lib/modules/$(KERNEL)/build M=$(PATH_SRC) clean

install:
	sudo insmod $(MODULE_NAME).ko
	@lsmod | grep "$(MODULE_NAME)"
	@echo "Done"

uninstall:
	sudo rmmod $(MODULE_NAME).ko
	@sudo rm -f /dev/$(MODULE_NAME)
	@sudo rm -f /proc/$(MODULE_NAME)
	@echo "Done"
