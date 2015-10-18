CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy
SOX=sox

CFLAGS= -std=gnu99 -ffreestanding \
		-mthumb -march=armv7-m -mcpu=cortex-m3 \
		-g -Wall -ggdb3
LDFLAGS=-nostdlib -lm -lgcc
ASFLAGS=-mthumb -march=armv7-m -mcpu=cortex-m3 -g
LINKERSCRIPT=ex2.ld

OBJECTS=vector.o crt.o ex2.o init.o gpio.o letimer.o waveforms.o sound.o
SOUNDS=sounds/wilhelm.o sounds/owl.o

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

ex2.elf : ${OBJECTS} ${SOUNDS}
	${CC} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

%.raw : %.wav
	${SOX} $< -r 32768 -b 8 -c 1 -e unsigned-integer $@

%.raw : %.ogg
	${SOX} $< -r 32768 -b 8 -c 1 -e unsigned-integer $@

%.o : %.raw
	${LD} -r -b binary $< -o $@
	${OBJCOPY} --rename-section .data=.rodata,readonly,data,content $@ $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex2.bin"; true

.PHONY : clean
clean :
	-rm -rf ${OBJECTS} ${SOUNDS} ex2.elf ex2.bin
