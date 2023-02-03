#
# Makefile for MPX Modules

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS = 
ASFLAGS = -f elf -g

# add new files as you create them.
# I'll start you off with just mpx_supt.o
OBJFILES =\
mpx_supt.o\
commandHandler.o\
versionAndShutdown.o\
getSetDateTime.o\
help.o\
data-structures/queue.o\
data-structures/PCB.o\
show.o\
PCBCommands.o\
procsr3.o\
loadr3.o\
alarm.o\
infinite.o\
mod5/mcb.o\
mod5/initializeHeap.o\
mod5/allocate_mem.o\
mod5/free_mem.o\
mod5/user_comms.o\
mod5/showmem.o\
mod6/io_scheduler.o\
mod6/DCB.o\
mod6/newTestProcs.o


.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: modules.o

modules.o: $(OBJFILES)
	$(LD) -r -o modules.o $(OBJFILES)

clean:
	rm -f modules.o $(OBJFILES)
