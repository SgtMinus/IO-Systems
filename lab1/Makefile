obj-m += ch_drv.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install:
	sudo insmod ch_drv.ko
	sudo chmod 777 /dev/var2
remove:
	sudo rmmod ch_drv
