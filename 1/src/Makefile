# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-ld
AS=arm-none-eabi-as --defsym DOSLEEP=1
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

ex1.bin : ex1.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1.elf : ex1.o vectors.o konami.o controller.o timer.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1.bin"; true

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
