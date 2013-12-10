
all: boot.o kernel.o screen.o tk421.bin

boot.o:
	nasm -f elf -o boot.o boot.s

kernel.o:
	i586-elf-gcc -c kernel.c -o kernel.o -fno-stack-protector -std=gnu99 -ffreestanding -O2 -Wall -Wextra

screen.o:
	i586-elf-gcc -c screen.c -o screen.o -fno-stack-protector -std=gnu99 -ffreestanding -O2 -Wall -Wextra

tk421.bin:
	i586-elf-gcc -T linker.ld -o tk421.bin -ffreestanding -O2 -nostdlib boot.o kernel.o screen.o -lgcc

clean:
	rm -f *.o *.bin
