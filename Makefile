#*******************************************************************************
#   ___   public
#  ¦OUX¦  GNU “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
.PHONY: init-qemu init-virtualbox init-vmware init-usb \
run-qemu-bios run-qemu-uefi
#===============================================================================
init-qemu:
	dd if=/dev/zero of=disk.img bs=1G count=1 \
    && parted -fs disk.img -a minimal 'mklabel gpt mkpart primary 0 1MB set 1 bios_grub on mkpart primary fat32 1MB 100MB set 2 esp on mkpart primary 100MB -0' \
    && loopdev=$$( losetup -LPf --show disk.img ) \
    && trap 'losetup -d $$loopdev' EXIT \
    && mkfs.fat -F 32 $${loopdev}p2 \
    && ../linux-ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p3 512
#-------------------------------------------------------------------------------
init-vmware:
	ocq_mnt=/mnt/oth; \
    mkdir -p $$ocq_mnt \
    && vmware-mount -f /mnt/hgfs/OUX_C+\ OS/OUX_C+\ OS.vmdk $$ocq_mnt \
    && trap 'vmware-mount -d $$ocq_mnt' EXIT \
    && parted -fs $$ocq_mnt/flat -a minimal 'mklabel gpt mkpart primary 0 1MB set 1 bios_grub on mkpart primary fat32 1MB 100MB set 2 esp on mkpart primary 100MB -0' \
    && loopdev=$$( losetup -LPf --show $$ocq_mnt/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $$ocq_mnt' EXIT \
    && mkfs.fat -F 32 $${loopdev}p2 \
    && ../linux-ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p3 512
init-virtualbox:
	ocq_mnt=/mnt/oth; \
    mkdir -p $$ocq_mnt \
    && $(VMWARE_DIR)/bin/vmware-mount -f ~inc/.VirtualBox/Machines/OUX_C+\ OS/OUX_C+\ OS.vmdk $$ocq_mnt \
    && trap '$(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
    && parted -fs $$ocq_mnt/flat -a minimal 'mklabel gpt mkpart primary 0 1MB set 1 bios_grub on mkpart primary fat32 1MB 100MB set 2 esp on mkpart primary 100MB -0' \
    && loopdev=$$( losetup -LPf --show $$ocq_mnt/flat ) \
    && trap 'losetup -d $$loopdev && $(VMWARE_DIR)/bin/vmware-mount -d $$ocq_mnt' EXIT \
    && mkfs.fat -F 32 $${loopdev}p2 \
    && ../linux-ouxfs-tools/mkfs-oux/mkfs.oux $${loopdev}p3 512
#-------------------------------------------------------------------------------
init-usb:
	ocq_dev=/dev/sdb \
    && parted -fs $$ocq_dev -a minimal 'mklabel gpt mkpart primary 0 1MB set 1 bios_grub on mkpart primary fat32 1MB 100MB set 2 esp on mkpart primary 100MB -0' \
    && mkfs.fat -F 32 $${ocq_dev}2 \
    && ../linux-ouxfs-tools/mkfs-oux/mkfs.oux $${ocq_dev}3 512
#-------------------------------------------------------------------------------
run-qemu-bios:
	qemu-system-x86_64 -machine q35 -cpu max -smp 24,cores=24 -m 32M,maxmem=32M \
    -drive media=disk,format=raw,file=disk.img -boot order=c,once=c
run-qemu-uefi:
	qemu-system-x86_64 -machine q35 -cpu max -smp 24,cores=24 -m 128M,maxmem=128M \
    -drive if=pflash,format=raw,readonly=on,file=OVMF_CODE_4M.fd \
    -drive if=pflash,format=raw,file=OVMF_VARS_4M.fd \
    -drive media=disk,format=raw,file=disk.img -boot order=c,once=c
#*******************************************************************************
