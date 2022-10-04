obj-m = myleds.o


all:
	make -C /home/se5/Zedboard/linux-xlnx-master/ M=$(PWD) modules
	arm-linux-gnueabihf-gcc -static -o test_myleds myleds.c
clean:
	make -C /home/se5/Zedboard/linux-xlnx-master/ M=$(PWD) clean

