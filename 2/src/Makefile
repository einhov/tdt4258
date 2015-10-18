CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

CFLAGS= -std=gnu99 -ffreestanding \
		-mthumb -march=armv7-m -mcpu=cortex-m3 \
		-g -Wall -ggdb3
LDFLAGS=-nostdlib -lm -lgcc
ASFLAGS=-mthumb -march=armv7-m -mcpu=cortex-m3 -g
LINKERSCRIPT=ex2.ld

OBJECTS=vector.o crt.o ex2.o init.o gpio.o letimer.o waveforms.o

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

ex2.elf : ${OBJECTS}
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex2.bin"; true

.PHONY : clean
clean :
	-rm -rf ${OBJECTS} ex2.elf ex2.bin
