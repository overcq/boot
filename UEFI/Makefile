#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  ‟GNU” “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   UEFI boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
AS := as
CC := clang
CFLAGS := -Os
#===============================================================================
all: build
build: fileloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build clean distclean install install-fileloader usb
#===============================================================================
disk.img:
	dd if=/dev/zero of=$@ bs=$$(( 2 * 18 * 512 )) count=80 \
	&& mkfs.fat $@ \
	&& chown inc:inc $@
fileloader: fileloader.h fileloader.c text.c Makefile
	$(CC) $(CFLAGS) -march=x86-64 -mno-avx -mno-red-zone -fcf-protection=none -ffreestanding -fshort-wchar -fno-stack-check -fno-stack-protector -fno-strict-aliasing -funsigned-char -fwrapv -Wall -Wextra -Wno-address-of-packed-member -Wno-pointer-sign -Wno-uninitialized -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Werror -nostdlib --target=x86_64-unknown-windows -fuse-ld=lld -Wl,-entry:H_uefi_I_main,-subsystem:efi_application -o $@ $(filter %.c,$^)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean: mostlyclean
	rm -f fileloader
distclean: clean
	rm -f disk.img
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: disk.img install-fileloader
install-fileloader: fileloader
	ocq_mnt=/mnt/oth; \
	loopdev=$$( losetup -LPf --show disk.img ); \
	trap 'losetup -d "$$loopdev"' EXIT; \
	mkdir -p $$ocq_mnt \
	&& mount "$$loopdev" $$ocq_mnt \
	|| exit 1; \
	trap 'umount $$ocq_mnt; losetup -d "$$loopdev"' EXIT; \
	mkdir -p $$ocq_mnt/EFI/BOOT \
	&& install fileloader $$ocq_mnt/EFI/BOOT/BOOTx64.EFI	
#-------------------------------------------------------------------------------
usb: fileloader
	ocq_usb_dev=/dev/sdc; \
	ocq_usb_mnt=/mnt/usb; \
	mount $${ocq_usb_dev}1 $$ocq_usb_mnt \
	|| exit 1; \
	trap 'umount $$ocq_usb_mnt' EXIT; \
	mkdir -p $$ocq_usb_mnt/EFI/BOOT \
	&& install fileloader $$ocq_usb_mnt/EFI/BOOT/BOOTx64.EFI
#*******************************************************************************
