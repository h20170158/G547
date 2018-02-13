obj-m := pit.o

all:
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) modules
install:
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) modules_install 
	depmod -a
clean:
	rm -f *~
	make -C /lib/modules/4.13.0-32-generic/build M=$(PWD) clean 
                                                                          
