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
VMWARE_DIR := /opt/vmware2
#===============================================================================
all: build
build: fileloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build clean distclean disk.img install usb
#===============================================================================
disk.img:
	ocq_mnt=/mnt/oth; \
	mkdir -p $$ocq_mnt \
	&& trap '$(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
	&& $(VMWARE_DIR)/bin/vmware-mount -f ~inc/vmware/boot\ UEFI/boot\ UEFI.vmdk $$ocq_mnt \
	&& parted $$ocq_mnt/flat \
	|| exit 1; \
	loopdev=$$( losetup -LPf --show $$ocq_mnt/flat ); \
	trap 'losetup -d $$loopdev && $(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT; \
	mkfs.fat $${loopdev}p1 \
	&& ~inc/moje/programy/ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p2 4096
fileloader: fileloader.h fileloader.c text.c ouxfs.c mem-blk.c Makefile
	$(CC) $(CFLAGS) -march=x86-64 -mno-avx -mno-red-zone -fcf-protection=none -ffreestanding -fshort-wchar -fno-stack-check -fno-stack-protector -fno-strict-aliasing -funsigned-char -fwrapv -Wall -Wextra -Wno-address-of-packed-member -Wno-dangling-else -Wno-pointer-sign -Wno-uninitialized -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Werror -nostdlib --target=x86_64-unknown-windows -fuse-ld=lld -Wl,-entry:H_uefi_I_main,-subsystem:efi_application -o $@ $(filter %.c,$^)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	rm -f fileloader
distclean: clean
	rm -f disk.img
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install:
	ocq_mnt=/mnt/oth; \
	mkdir -p $$ocq_mnt \
	&& trap '$(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
	&& $(VMWARE_DIR)/bin/vmware-mount ~inc/vmware/boot\ UEFI/boot\ UEFI.vmdk 1 $$ocq_mnt \
	|| exit 1; \
	mkdir -p $$ocq_mnt/EFI/BOOT \
	&& install fileloader $$ocq_mnt/EFI/BOOT/BOOTX64.EFI	
#-------------------------------------------------------------------------------
usb: fileloader
	ocq_usb_dev=/dev/sdc; \
	ocq_usb_mnt=/mnt/usb; \
	mount $${ocq_usb_dev}1 $$ocq_usb_mnt \
	|| exit 1; \
	trap 'umount $$ocq_usb_mnt' EXIT; \
	mkdir -p $$ocq_usb_mnt/EFI/BOOT \
	&& install fileloader $$ocq_usb_mnt/EFI/BOOT/BOOTX64.EFI
#*******************************************************************************
