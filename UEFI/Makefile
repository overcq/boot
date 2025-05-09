#*******************************************************************************
#   ___   public
#  ¦OUX¦  GNU “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   UEFI boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
CC := clang
CFLAGS := -Os
VMWARE_DIR := /opt/vmware2
#===============================================================================
all: build
build: fileloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build clean distclean init-qemu install-qemu init-vmware install-vmware install-usb run-qemu
#===============================================================================
disk.img:
	dd if=/dev/zero of=$@ bs=1M count=64 \
	&& chown inc:inc $@ \
	|| exit 1; \
	loopdev=$$( losetup -LPf --show $@ ); \
	trap 'losetup -d $$loopdev' EXIT \
	&& parted $$loopdev \
	&& mkfs.fat $${loopdev}p1 \
	&& ~inc/moje/programy/linux-ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p2 4096
fileloader: simple.h fileloader.h fileloader.c text.c ouxfs.c mem-blk.c Makefile
	$(CC) $(CFLAGS) -std=gnu23 -march=x86-64 -mno-avx -mno-red-zone -fcf-protection=none -ffreestanding -fshort-wchar -fno-stack-check -fno-stack-protector -fno-strict-aliasing -funsigned-char -fwrapv -Wall -Wextra -Wno-address-of-packed-member -Wno-dangling-else -Wno-parentheses -Wno-pointer-sign -Wno-switch -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Werror -nostdlib --target=x86_64-unknown-windows -fuse-ld=lld -Wl,-entry:H_uefi_I_main,-subsystem:efi_application -o $@ $(filter %.c,$^)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clean:
	rm -f fileloader
distclean: clean
	rm -f disk.img
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
init-qemu:
	dd if=/dev/zero of=disk.img bs=1M count=1000 \
	&& parted -fs disk.img mklabel gpt mkpart primary fat32 0 100MB set 1 esp on mkpart primary 100MB 1GB \
	&& loopdev=$$( losetup -LPf --show disk.img ) \
	&& trap 'losetup -d $$loopdev' EXIT \
	&& mkfs.fat -F 32 $${loopdev}p1 \
	&& ../../linux-ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p2 4096
install-qemu:
	ocq_mnt=/mnt/oth; \
	mkdir -p $$ocq_mnt \
	&& loopdev=$$( losetup -LPf --show disk.img ) \
	&& mount $${loopdev}p1 $$ocq_mnt \
	&& trap 'losetup -d $$loopdev' EXIT \
	&& trap 'losetup -d $$loopdev && umount $$ocq_mnt' EXIT \
	&& mkdir -p $$ocq_mnt/EFI/BOOT \
	&& install fileloader $$ocq_mnt/EFI/BOOT/BOOTX64.EFI
#-------------------------------------------------------------------------------
init-vmware:
	ocq_mnt=/mnt/oth; \
	mkdir -p $$ocq_mnt \
	&& $(VMWARE_DIR)/bin/vmware-mount -f ~inc/vmware/boot\ UEFI/boot\ UEFI.vmdk $$ocq_mnt \
	&& trap '$(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
	&& parted -fs $$ocq_mnt/flat mklabel gpt mkpart primary fat32 0 100MB set 1 esp on mkpart primary 100MB 8GB \
	&& loopdev=$$( losetup -LPf --show $$ocq_mnt/flat ) \
	&& trap 'losetup -d $$loopdev && $(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
	&& mkfs.fat -F 32 $${loopdev}p1 \
	&& ../../linux-ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p2 4096
install-vmware:
	ocq_mnt=/mnt/oth; \
	mkdir -p $$ocq_mnt \
	&& $(VMWARE_DIR)/bin/vmware-mount ~inc/vmware/boot\ UEFI/boot\ UEFI.vmdk 1 $$ocq_mnt \
	&& trap '$(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
	&& mkdir -p $$ocq_mnt/EFI/BOOT \
	&& install fileloader $$ocq_mnt/EFI/BOOT/BOOTX64.EFI
#-------------------------------------------------------------------------------
install-usb:
	ocq_usb_dev=/dev/sdc; \
	ocq_usb_mnt=/mnt/usb; \
	mount $${ocq_usb_dev}1 $$ocq_usb_mnt \
	&& trap 'umount $$ocq_usb_mnt' EXIT \
	&& mkdir -p $$ocq_usb_mnt/EFI/BOOT \
	&& install fileloader $$ocq_usb_mnt/EFI/BOOT/BOOTX64.EFI
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
run-qemu:
	qemu-system-x86_64 -s -S -nic none -machine q35 -cpu IvyBridge -smp 4,cores=2,threads=2 -m 128 \
	-drive if=pflash,format=raw,readonly=on,file=RELEASEX64_OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=RELEASEX64_OVMF_VARS.fd \
	-drive media=disk,format=raw,file=disk.img
#*******************************************************************************
