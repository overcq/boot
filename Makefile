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
.PHONY: all build mbr clean distclean install install-mbr install-vbr install-fileloader usb dump
#===============================================================================
disk.img:
	dd if=/dev/zero of=$@ bs=512 count=2812 \
	&& fdisk $@
mbr: mbr.S
	$(AS) -o a.out $< \
	&& $(LD) --oformat binary -Ttext 0x7a00 -o $@ a.out \
	&& rm a.out
vbr: vbr.S
	$(AS) -o a.out $< \
	&& $(LD) --oformat binary -Ttext 0x7c00 -o $@ a.out \
	&& rm a.out
fileloader-1: fileloader.S
	$(AS) -o a.out $< \
	&& $(LD) --oformat binary -Ttext 0x7e00 -o $@ a.out \
	&& rm a.out
fileloader-2: fileloader.c
	init_size=$$( stat -c %s fileloader-1 ); \
	$(CC) -c -ffreestanding -Os -o a.out $< \
	&& $(LD) --oformat binary -Ttext $$(( 0x7e00 + $$init_size )) -o $@ a.out \
	&& rm a.out
fileloader: fileloader-1 fileloader-2
	cat fileloader-1 fileloader-2 > $@ \
	&& rm $^ \
	&& { size=$$( stat -c %s $@ ); if [ $$size -lt 4096 ]; then dd conv=notrunc if=/dev/zero of=$@ bs=1 seek=$$size count=$$(( 4096 - $$size )); fi; }
install/a.out: install/main.cx
	$(MAKE) -C install
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	rm -f mbr vbr fileloader
distclean: clean
	rm -f disk.img
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: disk.img install-mbr install-vbr install-fileloader
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
	[ -n "$$loopdev" ] || { echo 1; exit 1; }; \
	trap 'losetup -d "$$loopdev"' EXIT; \
	[ -e "$${loopdev}p1" ] || { echo 2; exit 1; }; \
	mkfs.oux "$${loopdev}p1" \
	&& mkdir -p /mnt/oth \
	&& ( mount.oux "$${loopdev}p1" /mnt/oth 2>/dev/null & ) \
	|| { echo 3; exit 1; }; \
	trap 'umount /mnt/oth; losetup -d "$$loopdev"' EXIT; \
	sleep 1; \
	mkdir -p /mnt/oth/boot \
	&& { install $< /mnt/oth/boot/loader || true; } \
	&& install/a.out "$$loopdev" /boot/loader
# NDFN Dla kernela Linuksa 5.11.11 “install” wraca z błędem zamknięcia pliku przy poprawnej jego instalacji, więc ‘workaround’: “true”.
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
	&& { echo mbr \#1; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1"; } \
	&& { echo mbr \#2; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_2"; } \
	&& dd if=vbr of="$$part_1" bs=1 count=224 \
	&& dd if=vbr of="$$part_2" bs=1 skip=228 count=$$(( 428 - 228 )) \
	&& dd if=vbr of="$$part_3" bs=1 skip=440 count=$$(( 510 - 440 )) \
	&& { echo vbr \#1; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1"; } \
	&& { echo vbr \#2; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_2"; } \
	&& { echo vbr \#3; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_3"; } \
	&& dd if=fileloader of="$$part_1" bs=512 count=2 \
	&& { echo fileloader \#1; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1"; }
#*******************************************************************************
