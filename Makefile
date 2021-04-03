#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  ‟GNU” “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   boot loader
#         program makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
AS=as
CC=/usr/lib/llvm/11/bin/clang
LD=ld
#===============================================================================
all: build
build: mbr vbr fileloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build mbr clean install install-mbr install-vbr install-fileloader usb dump
#===============================================================================
mbr: mbr.S
	$(AS) -o a.out $< \
	&& $(LD) --oformat binary -Ttext 0x7a00 -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( 0x7a00 / 512 )) count=1 \
	&& rm $@_
vbr: vbr.S
	$(AS) -o a.out $< \
	&& $(LD) --oformat binary -Ttext 0x7c00 -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( 0x7c00 / 512 )) count=1 \
	&& rm $@_
fileloader-1: fileloader.S
	$(AS) -o a.out $< \
	&& $(LD) --oformat binary -Ttext 0x7e00 -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( 0x7e00 / 512 )) \
	&& rm $@_
fileloader-2: fileloader.c
	init_size=$$( stat -c %s fileloader-1 ); \
	$(CC) -c -ffreestanding -o a.out $< \
	&& $(LD) --oformat binary -Ttext $$(( 0x7e00 + $$init_size )) -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( ( 0x7e00 + $$init_size ) / 512 )) \
	&& rm $@_
fileloader: fileloader-1 fileloader-2
	cat fileloader-1 fileloader-2 > $@ \
	&& rm $^ \
	&& { size=$$( stat -c %s $@ ); if [ $$size -lt 4096 ]; then dd conv=notrunc if=/dev/zero of=$@ bs=1 seek=$$size count=$$(( 4096 - $$size )); fi; }
install/a.out: install/main.cx
	$(MAKE) -C install
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	rm -f mbr vbr
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: install-mbr install-vbr install-fileloader
install-mbr: mbr
	dd conv=notrunc if=$< of=disk.img bs=1 count=218 \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=510 seek=510 count=2
install-vbr: vbr
	dd conv=notrunc if=$< of=disk.img bs=1 seek=512 count=224 \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=228 seek=$$(( 512 + 228 )) count=$$(( 428 - 228 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=440 seek=$$(( 512 + 440 )) count=$$(( 512 - 440 ))
install-fileloader: fileloader install/a.out
	loopdev=$$( losetup -Pf --show disk.img ); \
	[ -n "$$loopdev" ] || exit 1; \
	trap 'losetup -d "$$loopdev"' EXIT; \
	[ -e "$${loopdev}p1" ] || exit 1; \
	mkfs.oux "$${loopdev}p1" \
	&& mkdir -p /mnt/oth \
	&& ( mount.oux "$${loopdev}p1" /mnt/oth 2>/dev/null & ) \
	&& sleep 1 \
	|| exit 1; \
	trap 'umount /mnt/oth; losetup -d "$$loopdev"' EXIT; \
	mkdir -p /mnt/oth/boot \
	&& install $< /mnt/oth/boot/loader \
	&& install/a.out "$${loopdev}p1" /boot/loader
#-------------------------------------------------------------------------------
usb: mbr vbr
	dd conv=notrunc if=mbr of=/dev/sdb bs=1 count=218 \
	&& dd conv=notrunc if=mbr of=/dev/sdb bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
	&& dd conv=notrunc if=vbr of=/dev/sdb bs=1 seek=512 count=428 \
	&& dd conv=notrunc if=vbr of=/dev/sdb bs=1 skip=440 seek=$$(( 512 + 440 )) count=$$(( 512 - 440 ))
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dump: mbr vbr
	part_1=$$( mktemp ); part_2=$$( mktemp ); part_3=$$( mktemp ); \
	trap 'rm "$$part_1" "$$part_2" "$$part_3"' EXIT; \
	dd if=mbr of="$$part_1" bs=1 count=218 \
	&& dd if=mbr of="$$part_2" bs=1 skip=228 count=$$(( 428 - 228 )) \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1" \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_2" \
	&& dd if=vbr of="$$part_1" bs=1 count=224 \
	&& dd if=vbr of="$$part_2" bs=1 skip=228 count=$$(( 428 - 228 )) \
	&& dd if=vbr of="$$part_3" bs=1 skip=440 count=$$(( 510 - 440 )) \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1" \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_2" \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_3" \
	&& objdump -D -b binary -mi386 fileloader
#*******************************************************************************
