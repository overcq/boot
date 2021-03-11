#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  ‟GNU” “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   MBR boot loader
#         program makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
all: build
build: mbr vbr
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build mbr clean install install-mbr install-vbr usb dump
#===============================================================================
mbr: mbr.S
	as -o a.out $< \
	&& ld --oformat binary -Ttext 0x7a00 -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( 0x7a00 / 512 )) count=1 \
	&& rm $@_
vbr: vbr.S
	as -o a.out $< \
	&& ld --oformat binary -Ttext 0x7c00 -o $@_ a.out \
	&& rm a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( 0x7c00 / 512 )) count=1 \
	&& rm $@_
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	rm -f mbr vbr
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: install-mbr install-vbr
install-mbr: mbr
	dd conv=notrunc if=$< of=disk.img bs=1 count=218 \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=224 seek=224 count=$$(( 440 - 224 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=510 seek=510 count=2
install-vbr: vbr
	dd conv=notrunc if=$< of=disk.img bs=512 seek=1 count=1
#-------------------------------------------------------------------------------
usb: mbr
	dd conv=notrunc if=$< of=/dev/sdb bs=1 count=218 \
	&& dd conv=notrunc if=$< of=/dev/sdb bs=1 skip=224 seek=224 count=$$(( 440 - 224 )) \
	&& dd conv=notrunc if=$< of=/dev/sdb bs=1 skip=510 seek=510 count=2
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dump: mbr vbr
	part_1=$$( mktemp ); part_2=$$( mktemp ); \
	trap EXIT 'rm "$$part_1" "$$part_2"'; \
	dd if=mbr of="$$part_1" bs=1 count=218 \
	&& dd if=mbr of="$$part_2" bs=1 skip=224 count=$$(( 440 - 224 )) \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_1" \
	&& objdump -D -b binary -mi386 -Maddr16,data16 "$$part_2" \
	&& objdump -D -b binary -mi386 -Maddr16,data16 vbr
#*******************************************************************************
