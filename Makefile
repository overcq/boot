#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  ‟GNU” “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   MBR boot loader
#         program makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
all: build
build: mbr
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build mbr mostlyclean clean install install-mbr usb dump
#===============================================================================
mbr: mbr.S
	as -o a.out $< \
	&& ld --oformat binary -Ttext 0x7a00 -o $@_ a.out \
	&& dd if=$@_ of=$@ bs=512 skip=$$(( 0x7a00 / 512 )) count=1 \
	&& rm -f $@_
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mostlyclean:
	rm -f a.out
clean: mostlyclean
	rm -f mbr
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: install-mbr
install-mbr: mbr
	dd conv=notrunc if=$< of=disk.img bs=1 count=218 \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=224 seek=224 count=$$(( 440 - 224 )) \
	&& dd conv=notrunc if=$< of=disk.img bs=1 skip=510 seek=510 count=2
#-------------------------------------------------------------------------------
usb: mbr
	dd conv=notrunc if=$< of=/dev/sdb bs=1 count=218 \
	&& dd conv=notrunc if=$< of=/dev/sdb bs=1 skip=224 seek=224 count=$$(( 440 - 224 )) \
	&& dd conv=notrunc if=$< of=/dev/sdb bs=1 skip=510 seek=510 count=2
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dump: mbr
	objdump -D -b binary -mi386 -Maddr16,data16 mbr
#*******************************************************************************
