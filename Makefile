
CC      := i586-elf-gcc
CFLAGS  := -fno-stack-protector -fno-builtin -nostdlib -std=gnu99 -ffreestanding -O2 -I.
LDFLAGS := -nostdlib -Wall -Wextra

all: boot.o kernel.o screen.o keyboard.o tk421.bin

tk421.bin:
	$(CC) -T linker.ld -o tk421.bin $(CFLAGS) $(LDFLAGS) boot.o kernel.o screen.o keyboard.o -lgcc

boot.o: boot.s
	nasm -f elf -o boot.o boot.s

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

screen.o: screen.c screen.h
	$(CC) $(CFLAGS) -c screen.c -o screen.o

keyboard.o: keyboard.c keyboard.h
	$(CC) $(CFLAGS) -c keyboard.c -o keyboard.o

clean:
	rm -f *.o *.bin
