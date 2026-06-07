OBJECTS = loader.o kmain.o io.o gdt.o gdt_flush.o interrupt.o interrupt_asm.o \
          pic.o keyboard.o framebuffer.o serial.o log.o

CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

LDFLAGS = -T link.ld -melf_i386

AS = nasm
ASFLAGS = -f elf32

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

program: program.s
	$(AS) -f bin program.s -o program

os.iso: kernel.elf program
	mkdir -p iso/modules
	cp program iso/modules/program
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
	            -b boot/grub/stage2_eltorito \
	            -no-emul-boot \
	            -boot-load-size 4 \
	            -A os \
	            -input-charset utf8 \
	            -quiet \
	            -boot-info-table \
	            -o os.iso \
	            iso

run: os.iso
	bochs -f bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso com1.out program iso/modules/program