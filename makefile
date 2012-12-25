
OUTELF=prog.elf
OUTLDR=prog.ldr
OUTHEX=prog.hex
SOURCES=entry.S main.c printf.c uart.c console.c

CFLAGS=-mcpu=bf532 -nostartfiles
CC=bfin-elf-gcc

all : $(OUTHEX)

clean :
	rm $(OUTELF) $(OUTLDR) $(OUTHEX) init.o

$(OUTELF) : $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTELF) $(SOURCES)

$(OUTLDR) : $(OUTELF) init.o
	bfin-elf-ldr -T bf532 -c $(OUTLDR) $(OUTELF) --initcode init.o

init.o : init.c
	$(CC) -c $(CFLAGS) -fno-builtin -o init.o init.c

$(OUTHEX) : $(OUTLDR)
#	srec_cat $(OUTLDR) -Binary -Output $(OUTHEX) -Intel
	bfin-elf-objcopy -I binary -O ihex $(OUTLDR) $(OUTHEX)
