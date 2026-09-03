# OUX/C+ OS boot loader

System uruchamia się zarówno przez BIOS jak i UEFI.

## BIOS

![OUX/C+ OS boot loader in qemu emulator](BIOS/OUX_C+-OS-boot-loader-qemu.png)

![OUX/C+ OS boot loader in Vmware emulator](BIOS/OUX_C+-OS-boot-loader-vmware.png)

![OUX/C+ OS boot loader in bochs emulator](BIOS/OUX_C+-OS-boot-loader-bochs.png)

### Specyfikacja

Dostępne są następujace moduły:

1. program Master Boot Record 〈mbr〉 (plik źródłowy: “mbr.S”)
2. Second Boot Loader 〈sbl〉 (“kernelloader.S”)
3. Kernel Boot Loader 〈kbl〉 (“kernelloader.c” i inne)

Proces uruchamiania przebiega w następujący sposób: ‹mbr› ➪ ‹sbl› ➪ ‹kbl›.

W ‹mbr› od bajtu nr 224 znajduje się 4 B informacji, gdzie położony jest i jaki ma rozmiar blok ‹sbl› + ‹kbl›: najpierw w 2 B w formacie LBA jest zapisane położenie bloku na dysku, a następnie w 2 B jest liczba sektorów o rozmiarze 512 B składająca się na rozmiar tego bloku.

Blok ‹sbl› + ‹kbl› jest typu ‘binary’ i zawiera doklejone informacje o relokacji.

Powinien on znajdować się na osobnej partycji typu “bios_grub”.

### Uwagi

Uruchamianie przez BIOS nie zostało jeszcze napisane. Obecnie uruchamia się tylko początkowa faza ‘boot loadera’. Potrzeba jeszcze ułożyć dane w pamięci i wczytać ‘kernel’.

## UEFI

Po uruchomieniu przez UEFI ‘boot loader’ wczytuje do pamięci operacyjnej z systemu plików OUXFS z pierwszego dostępnego dysku, na którym on się znajdzie, plik “/system/kernel”. Następnie przygotowuje dane z tablic ACPI, dokonuje reorganizacji ‘mapowania’ pamięci wirtualnej tak, by obszary zarezerwowanej pamięci były dosunięte do dołu lub góry pamięci rzeczywistej, uruchamia menedżer pamięci ‘mem-blk’ i przekazuje sterowanie do ‘kernela’ z danymi.
