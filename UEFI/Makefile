#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  ‟GNU” “make”
#  ¦/C+¦  OUX/C+ OS
#   ---   UEFI boot loader
#         program makefile
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
AS := as
CC := clang
LD := ld
CFLAGS := -Os -fwrapv
#===============================================================================
all: build
build: fileloader
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.PHONY: all build mostlyclean clean distclean install install-fileloader usb
#===============================================================================
disk.img:
	dd if=/dev/zero of=$@ bs=$$(( 2 * 18 * 512 )) count=80 \
	&& mkfs.fat $@ \
	&& chown inc:inc $@
%.o: %.c fileloader.h Makefile
	$(CC) $(CFLAGS) -m64 -march=x86-64 -mno-red-zone -mno-sse -ffreestanding -fpic -fshort-wchar -fno-stack-protector -c -o $@ $<
fileloader: fileloader.o Makefile efi.ld
	$(LD) -T efi.ld -Bsymbolic -nostdlib -z nocombreloc --warn-common --fatal-warnings -shared -s -o $@ $(filter %.o,$^) \
	&& objcopy -j .text -j .data -j .reloc --target=efi-app-x86_64 $@
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mostlyclean:
	rm -f fileloader.o
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
