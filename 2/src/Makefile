CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

CFLAGS= -std=c99 -ffreestanding \
		-mthumb -march=armv7-m -mcpu=cortex-m3 \
		-g -Wall -fno-eliminate-unused-debug-types -fno-eliminate-unused-debug-symbols -ggdb3
LDFLAGS=-nostdlib -lgcc
ASFLAGS=-mthumb -march=armv7-m -mcpu=cortex-m3 -g
LINKERSCRIPT=ex2.ld

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

ex2.elf : vector.o crt.o ex2.o init.o gpio.o letimer.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex2.bin"; true

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex 
