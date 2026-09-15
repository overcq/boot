#*******************************************************************************
#   ___   public
#  ¦OUX¦  GNU “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   UEFI boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
include ../env.mk
#===============================================================================
CC := clang
CFLAGS := -Oz
#===============================================================================
all: build
build: kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build clean install-qemu install-virtualbox install-vmware
#===============================================================================
kernelloader: simple.h kernelloader.h $(wildcard *.c) remap_jump.S mp-init.S Makefile
	$(CC) $(CFLAGS) -std=c23 -march=x86-64 -mno-sse -mno-red-zone -mno-stack-arg-probe -fcf-protection=none -ffreestanding -fno-asynchronous-unwind-tables -fshort-wchar -fno-stack-check -fno-stack-protector -fno-strict-aliasing -funsigned-char -fno-unwind-tables -fwrapv -Wall -Wextra -Wno-address-of-packed-member -Wno-dangling-else -Wno-parentheses -Wno-sign-compare -Wno-switch -nostdlib --target=x86_64-unknown-windows -fuse-ld=lld -Wl,-entry:H_uefi_I_main,-subsystem:efi_application -o $@ $(filter %.c,$^) $(filter %.S,$^)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	-rm kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install-virtualbox:
	vmware-mount $(H_ocq_S_virtualbox_disk) 2 $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && mkdir -p $(H_ocq_S_mnt)/EFI/BOOT \
    && install kernelloader $(H_ocq_S_mnt)/EFI/BOOT/BOOTX64.EFI
install-vmware:
	vmware-mount $(H_ocq_S_vmware_disk) 2 $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && mkdir -p $(H_ocq_S_mnt)/EFI/BOOT \
    && install kernelloader $(H_ocq_S_mnt)/EFI/BOOT/BOOTX64.EFI
install-qemu:
	loopdev=$$( losetup -LPf --show ../disk.img ) \
    && trap 'losetup -d $$loopdev' EXIT \
    && mount $${loopdev}p2 $(H_ocq_S_mnt) \
    && trap 'losetup -d $$loopdev && umount $(H_ocq_S_mnt)' EXIT \
    && mkdir -p $(H_ocq_S_mnt)/EFI/BOOT \
    && install kernelloader $(H_ocq_S_mnt)/EFI/BOOT/BOOTX64.EFI
#*******************************************************************************
