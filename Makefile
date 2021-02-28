
all: build
build: mbr

.PHONY: all build mbr mostlyclean clean usb dump

mbr: mbr.S
	as -o a.out $<
	ld --oformat binary -Ttext 0x7a00 -o $@_ a.out
	dd if=$@_ of=$@ bs=512 skip=$$(( 0x7a00 / 512 )) count=1
	rm -f $@_

mostlyclean:
	rm -f a.out

clean: mostlyclean
	rm -f mbr

usb: mbr
	dd if=$< of=/dev/sdb bs=512 count=1

dump:
	objdump -D -b binary -mi386 -Maddr16,data16 mbr
