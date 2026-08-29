#*******************************************************************************
#   ___   public
#  ¦OUX¦  GNU “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   BIOS boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
include ../env.mk
#===============================================================================
CC := clang
CFLAGS := -Oz
#===============================================================================
all: build
build: mbr kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build mostlyclean clean \
install-qemu install-mbr-qemu install-kernelloader-qemu \
install-vmware install-mbr-vmware install-kernelloader-vmware \
install-usb install-mbr-usb install-kernelloader-usb
#===============================================================================
mbr: mbr.S \
Makefile mbr.ld
	$(CC) -m64 -fno-pic -static -nostdlib -Wl,-T,mbr.ld -o $@ $<
%.o: %.c kernelloader.h \
simple.h \
Makefile
	$(CC) $(CFLAGS) -std=c23 -m64 -march=x86-64 -mno-avx -mno-sse -mno-red-zone -mno-stack-arg-probe -fcf-protection=none -fno-asynchronous-unwind-tables -ffreestanding -fno-stack-check -fno-stack-protector -fno-unwind-tables -fno-zero-initialized-in-bss -fwrapv -Wall -Wextra -Wno-address-of-packed-member -Wno-dangling-else -Wno-parentheses -Wno-sign-compare -Wno-switch -fno-pic -static -nostdlib -c -o $@ $<
kernelloader: kernelloader.S $(filter-out ouxfs.o,$(patsubst %.c,%.o,$(wildcard *.c))) \
Makefile kernelloader.ld
	$(CC) -c -o a.out kernelloader.S \
    && $(CC) -m64 -fno-pic -static -nostdlib -Wl,-T,kernelloader.ld -o $@ a.out $(filter %.o,$^) \
    && rm a.out
install/a.out: install/main.cx
	$(MAKE) -C install
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mostlyclean:
	-rm *.o
clean: mostlyclean
	-rm mbr kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install-qemu: install-mbr-qemu install-kernelloader-qemu
install-mbr-qemu: mbr
	loopdev=$$( losetup -Lf --show ../disk.img ) \
    && trap 'losetup -d $$loopdev' EXIT \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 count=218 \
    && dd conv=notrunc if=/dev/zero of=$$loopdev bs=1 seek=224 count=$$(( 228 - 224 )) \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 skip=510 seek=510 count=2
install-kernelloader-qemu: kernelloader install/a.out
	loopdev=$$( losetup -LPf --show ../disk.img ) \
    && trap 'losetup -d $$loopdev' EXIT \
    && [ -e $${loopdev}p1 ] \
    && dd if=kernelloader of=$${loopdev}p1 \
    && install/a.out $$loopdev kernelloader
install-virtualbox: install-mbr-vmware install-kernelloader-vmware
install-mbr-virtualbox: mbr
	vmware-mount -f $(H_ocq_S_virtualbox_disk) $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && loopdev=$$( losetup -Lf --show $(H_ocq_S_mnt)/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 count=218 \
    && dd conv=notrunc if=/dev/zero of=$$loopdev bs=1 seek=224 count=$$(( 228 - 224 )) \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 skip=510 seek=510 count=2
install-kernelloader-virtualbox: kernelloader install/a.out
	vmware-mount -f $(H_ocq_S_virtualbox_disk) $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && loopdev=$$( losetup -LPf --show $(H_ocq_S_mnt)/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && [ -e $${loopdev}p1 ] \
    && dd if=kernelloader of=$${loopdev}p1 \
    && install/a.out $$loopdev kernelloader
install-vmware: install-mbr-vmware install-kernelloader-vmware
install-mbr-vmware: mbr
	vmware-mount -f $(H_ocq_S_vmware_disk) $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && loopdev=$$( losetup -Lf --show $(H_ocq_S_mnt)/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 count=218 \
    && dd conv=notrunc if=/dev/zero of=$$loopdev bs=1 seek=224 count=$$(( 228 - 224 )) \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
    && dd conv=notrunc if=$< of=$$loopdev bs=1 skip=510 seek=510 count=2
install-kernelloader-vmware: kernelloader install/a.out
	vmware-mount -f $(H_ocq_S_vmware_disk) $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && loopdev=$$( losetup -LPf --show $(H_ocq_S_mnt)/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && [ -e $${loopdev}p1 ] \
    && dd if=kernelloader of=$${loopdev}p1 \
    && install/a.out $$loopdev kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install-usb: install-mbr-usb install-kernelloader-usb
install-mbr-usb: mbr
	[ -e $(H_ocq_S_usb_dev) ] \
    && dd if=$< of=$(H_ocq_S_usb_dev) bs=1 count=218 \
    && dd if=/dev/zero of=$(H_ocq_S_usb_dev) bs=1 seek=224 count=$$(( 228 - 224 )) \
    && dd if=$< of=$(H_ocq_S_usb_dev) bs=1 skip=228 seek=228 count=$$(( 440 - 228 )) \
    && dd if=$< of=$(H_ocq_S_usb_dev) bs=1 skip=510 seek=510 count=2
install-kernelloader-usb: kernelloader install/a.out
	[ -e $(H_ocq_S_usb_dev)1 ] \
    && dd if=kernelloader of=$(H_ocq_S_usb_dev)1 \
    && install/a.out $(H_ocq_S_usb_dev) kernelloader
#*******************************************************************************
