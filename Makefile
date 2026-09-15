#*******************************************************************************
#   ___   public
#  ¦OUX¦  GNU “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   boot loader
#         makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
include env.mk
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
H_ocq_S_parted_cmd := mklabel gpt mkpart primary 0 1023s set 1 bios_grub on mkpart primary fat32 1024s 100MB set 2 esp on mkpart primary 100MB -0
#===============================================================================
.PHONY: init-qemu init-virtualbox init-vmware init-usb \
run-qemu-bios run-qemu-uefi
#===============================================================================
init-virtualbox:
	vmware-mount -f $(H_ocq_S_virtualbox_disk) $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && parted -fs $(H_ocq_S_mnt)/flat -a minimal '$(H_ocq_S_parted_cmd)' \
    && loopdev=$$( losetup -LPf --show $(H_ocq_S_mnt)/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && mkfs.fat -F 32 $${loopdev}p2 \
    && $(H_ocq_S_mkfs_oux)/mkfs.oux $${loopdev}p3 512
init-vmware:
	vmware-mount -f $(H_ocq_S_vmware_disk) $(H_ocq_S_mnt) \
    && trap 'vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && parted -fs $(H_ocq_S_mnt)/flat -a minimal '$(H_ocq_S_parted_cmd)' \
    && loopdev=$$( losetup -LPf --show $(H_ocq_S_mnt)/flat ) \
    && trap 'losetup -d $$loopdev && vmware-mount -d $(H_ocq_S_mnt)' EXIT \
    && mkfs.fat -F 32 $${loopdev}p2 \
    && $(H_ocq_S_mkfs_oux)/mkfs.oux $${loopdev}p3 512
init-qemu:
	dd if=/dev/zero of=disk.img bs=1G count=1 \
    && parted -fs disk.img -a minimal '$(H_ocq_S_parted_cmd)' \
    && loopdev=$$( losetup -LPf --show disk.img ) \
    && trap 'losetup -d $$loopdev' EXIT \
    && mkfs.fat -F 32 $${loopdev}p2 \
    && $(H_ocq_S_mkfs_oux)/mkfs.oux $${loopdev}p3 512
#-------------------------------------------------------------------------------
init-usb:
	parted -fs $(H_ocq_S_usb_dev) -a minimal '$(H_ocq_S_parted_cmd)' \
    && mkfs.fat -F 32 $(H_ocq_S_usb_dev)2 \
    && $(H_ocq_S_mkfs_oux)/mkfs.oux $(H_ocq_S_usb_dev)3 512
#-------------------------------------------------------------------------------
run-qemu-bios:
	qemu-system-x86_64 -machine q35 -cpu IvyBridge -smp 4,cores=2 -m 2G,maxmem=2G \
    -device VGA,vgamem_mb=8 \
    -boot order=c,once=c \
    -drive media=disk,format=raw,file=disk.img,if=none,id=disc0 \
    -device ahci,id=ahci0 -device ide-hd,drive=disc0,bus=ahci0.0
run-qemu-uefi:
	qemu-system-x86_64 -machine q35 -cpu max -smp 24,cores=24 -m 2G,maxmem=2G \
    -drive if=pflash,format=raw,readonly=on,file=code.fd \
    -drive if=pflash,format=raw,file=vars.fd \
    -boot order=c,once=c \
    -drive media=disk,format=raw,file=disk.img,if=none,id=disc0 \
    -device ahci,id=ahci0 -device ide-hd,drive=disc0,bus=ahci0.0
#*******************************************************************************

