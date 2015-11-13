#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
OBJCOPY=arm-cortexm3-uclinuxeabi-objcopy
CFLAGS += -std=gnu99 -O2 -g -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=

DOGES = 1.o 2.o 3.o 4.o 5.o 6.o 7.o 8.o 9.o 10.o 11.o 12.o 13.o 14.o 15.o 16.o 17.o 18.o flipped.o

all: game

game : game.o graphics.o puzzle.o ${DOGES}
	${CC} $^ -o $@ ${LDFLAGS}

%.o : %.raw
	${LD} -r -b binary $^ -o $@
	${OBJCOPY} --rename-section .data=.rodata,readonly,data,content $@ $@

clean:
	-rm -f game *.o

install:

.PHONY: all install clean

# end of development makefile
