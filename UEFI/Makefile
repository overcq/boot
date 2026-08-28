#*******************************************************************************
#   ___   public
#  ¦OUX¦  GNU “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   UEFI boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
CC := clang
CFLAGS := -Oz
#===============================================================================
all: build
build: kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build clean install-qemu install-virtualbox install-vmware
#===============================================================================
kernelloader: simple.h kernelloader.h $(wildcard *.c) remap_jump.S mp-init.S Makefile
	$(CC) $(CFLAGS) -std=c23 -march=x86-64 -mno-avx -mno-red-zone -mno-stack-arg-probe -fcf-protection=none -ffreestanding -fno-asynchronous-unwind-tables -fshort-wchar -fno-stack-check -fno-stack-protector -fno-strict-aliasing -funsigned-char -fno-unwind-tables -fwrapv -Wall -Wextra -Wno-address-of-packed-member -Wno-dangling-else -Wno-parentheses -Wno-sign-compare -Wno-switch -nostdlib --target=x86_64-unknown-windows -fuse-ld=lld -Wl,-entry:H_uefi_I_main,-subsystem:efi_application -o $@ $(filter %.c,$^) $(filter %.S,$^)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	-rm kernelloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install-qemu:
	ocq_mnt=/mnt/oth; \
    mkdir -p $$ocq_mnt \
    && loopdev=$$( losetup -LPf --show ../disk.img ) \
    && trap 'losetup -d $$loopdev' EXIT \
    && mount $${loopdev}p2 $$ocq_mnt \
    && trap 'losetup -d $$loopdev && umount $$ocq_mnt' EXIT \
    && mkdir -p $$ocq_mnt/EFI/BOOT \
    && install kernelloader $$ocq_mnt/EFI/BOOT/BOOTX64.EFI
#-------------------------------------------------------------------------------
install-vmware:
	ocq_mnt=/mnt/oth; \
    mkdir -p $$ocq_mnt \
    && vmware-mount /mnt/hgfs/OUX_C+\ OS/OUX_C+\ OS.vmdk 2 $$ocq_mnt \
    && trap 'vmware-mount -d $$ocq_mnt' EXIT \
    && mkdir -p $$ocq_mnt/EFI/BOOT \
    && install kernelloader $$ocq_mnt/EFI/BOOT/BOOTX64.EFI
install-virtualbox:
	ocq_mnt=/mnt/oth; \
    mkdir -p $$ocq_mnt \
    && $(VMWARE_DIR)/bin/vmware-mount ~inc/.VirtualBox/Machines/OUX_C+\ OS/OUX_C+\ OS.vmdk 2 $$ocq_mnt \
    && trap '$(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
    && mkdir -p $$ocq_mnt/EFI/BOOT \
    && install kernelloader $$ocq_mnt/EFI/BOOT/BOOTX64.EFI
#*******************************************************************************
