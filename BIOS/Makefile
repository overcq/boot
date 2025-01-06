#*******************************************************************************
#   ___   publicplacex
#  ¦OUX¦  ‟GNU” “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   boot loader
#         program makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
AS := as
CC := clang
LD := ld
CFLAGS := -Os -fwrapv
#===============================================================================
all: build
build: mbr vbr fileloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build mostlyclean clean distclean install install-mbr install-vbr install-fileloader usb dump
#===============================================================================
disk.img:
	dd if=/dev/zero of=$@ bs=$$(( 2 * 18 * 512 )) count=80 \
	&& fdisk $@ \
	&& chown inc:inc $@
mbr: mbr.S Makefile binary.ld
	$(AS) -o a.out $< \
	&& $(LD) -T binary.ld --oformat binary -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ skip=$$(( 0x7a00 / 512 )) \
	&& rm $@_
vbr: vbr.S Makefile binary.ld
	$(AS) -o a.out $< \
	&& $(LD) -T binary.ld --oformat binary -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ skip=$$(( 0x7c00 / 512 )) \
	&& rm $@_
%.o: %.c fileloader.h simple.h Makefile
	$(CC) $(CFLAGS) -m64 -march=x86-64 -mno-sse -ffreestanding -fno-stack-protector -c -o $@ $<
fileloader: fileloader.S fileloader.o mem-blk.o vga.o font.o Makefile binary.ld
	$(AS) -o a.out fileloader.S \
	&& $(LD) -T binary.ld --oformat binary -o $@_ a.out $(filter %.o,$^) \
	&& rm a.out \
	&& dd if=$@_ of=$@ skip=$$(( 0x7e00 / 512 )) \
	&& rm $@_
install/a.out: install/main.cx
	$(MAKE) -C install
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mostlyclean:
	rm -f fileloader.o mem-blk.o vga.o font.o
clean:
	rm -f mbr vbr fileloader
distclean: clean
	rm -f disk.img
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: disk.img install-mbr install-vbr install-fileloader
install-mbr: mbr
	dd conv=notrunc if=$< of=disk.img bs=1 count=218 \
	&& dd conv=notrunc if=/dev/zero of=disk.img bs=1 seek=224 count=$$(( 228 - 224 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=510 seek=510 count=2
install-vbr: vbr
	start=$$(( $$( fdisk -l disk.img | tail -n 1 | sed -e 's`^[^ ][^ ]*  *\*  *\([0-9][0-9]*\).*$$`\1`' ) * 512 )); \
	dd conv=notrunc if=$< of=disk.img bs=1 seek=$$start count=224 \
	&& dd conv=notrunc if=/dev/zero of=disk.img bs=1 seek=$$(( $$start + 224 )) count=$$(( 228 - 224 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=228 seek=$$(( $$start + 228 )) count=$$(( 428 - 228 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=440 seek=$$(( $$start + 440 )) count=$$(( 512 - 440 ))
install-fileloader: fileloader install/a.out
	ocq_mnt=/mnt/oth; \
	loopdev=$$( losetup -LPf --show disk.img ); \
	[ -n "$$loopdev" ] || exit 1; \
	trap 'losetup -d "$$loopdev"' EXIT; \
	[ -e "$${loopdev}p1" ] || exit 1; \
	oux mkfs.oux "$${loopdev}p1" \
	&& mkdir -p $$ocq_mnt \
	&& ( oux mount.oux "$${loopdev}p1" $$ocq_mnt 2>/dev/null & ) \
	|| exit 1; \
	trap 'umount $$ocq_mnt; losetup -d "$$loopdev"' EXIT; \
	sleep 1; \
	mkdir -p $$ocq_mnt/boot \
	&& install fileloader $$ocq_mnt/boot/loader \
	&& oux install/a.out -o disk "$$loopdev" /boot/loader
#-------------------------------------------------------------------------------
usb: mbr vbr fileloader install/a.out
	ocq_usb_dev=/dev/sdb; \
	ocq_usb_mnt=/mnt/usb; \
	start=$$(( $$( fdisk -l $$ocq_usb_dev | tail -n 1 | sed -e 's`^[^ ][^ ]*  *\*  *\([0-9][0-9]*\).*$$`\1`' ) * 512 )); \
	dd conv=notrunc if=mbr of=$$ocq_usb_dev bs=1 count=218 \
	&& dd conv=notrunc if=/dev/zero of=$$ocq_usb_dev bs=1 seek=224 count=$$(( 228 - 224 )) \
	&& dd conv=notrunc if=mbr of=$$ocq_usb_dev bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
	&& dd conv=notrunc if=vbr of=$$ocq_usb_dev bs=1 seek=$$start count=224 \
	&& dd conv=notrunc if=/dev/zero of=$$ocq_usb_dev bs=1 seek=$$(( $$start + 224 )) count=$$(( 228 - 224 )) \
	&& dd conv=notrunc if=vbr of=$$ocq_usb_dev bs=1 skip=228 seek=$$(( $$start + 228 )) count=$$(( 428 - 228 )) \
	&& dd conv=notrunc if=vbr of=$$ocq_usb_dev bs=1 skip=440 seek=$$(( $$start + 440 )) count=$$(( 512 - 440 )) \
	|| exit 1; \
	sleep 4; \
	mkfs.oux $${ocq_usb_dev}1 \
	&& mkdir -p $$ocq_usb_mnt \
	&& ( mount.oux $${ocq_usb_dev}1 $$ocq_usb_mnt 2>/dev/null & ) \
	|| exit 1; \
	trap 'umount $$ocq_usb_mnt' EXIT; \
	sleep 4; \
	mkdir -p $$ocq_usb_mnt/boot \
	&& install fileloader /mnt/usb/boot/loader \
	&& install/a.out $$ocq_usb_dev /boot/loader
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
	&& dd if=fileloader of="$$part_1" bs=1 count=256 \
	&& dd if=fileloader of="$$part_2" bs=1 skip=256 count=256 \
	&& dd if=fileloader of="$$part_3" bs=1 skip=256 \
	&& { echo fileloader \#1; objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1"; } \
	&& { echo fileloader \#2; objdump -D -b binary -mi386 -Maddr32,data32 "$$part_2"; } \
	&& { echo fileloader \#3; objdump -D -b binary -mi386 -Mx86-64 "$$part_3"; }
#*******************************************************************************
