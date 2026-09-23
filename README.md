# OUX/C+ OS boot loader

System uruchamia się zarówno przez BIOS jak i UEFI.

## BIOS

Dostępne są następujace moduły:

1. program Master Boot Record 〈mbr〉 (plik źródłowy: “mbr.S”)
2. Second Boot Loader 〈sbl〉 (“kernelloader.S”)
3. Kernel Boot Loader 〈kbl〉 (“kernelloader.c” i inne)

Proces uruchamiania przebiega w następujący sposób: ‹mbr› ➪ ‹sbl› ➪ ‹kbl› ➪ ‘kernel’.

W ‹mbr› od bajtu nr 224 znajduje się 4 B informacji, gdzie jest położony i jaki ma rozmiar blok ‹sbl› + ‹kbl›: najpierw w 2 B w formacie LBA jest zapisane położenie bloku na dysku, a następnie w 2 B jest liczba sektorów o rozmiarze 512 B składająca się na rozmiar tego bloku.

Blok ‹sbl› + ‹kbl› jest typu ‘binary’ i zawiera doklejone informacje o relokacji.

Powinien znajdować się na osobnej partycji typu “bios_grub”.

## UEFI

‘Boot loader’ jest w postaci ‘OS loadera’ (rodzaj aplikacji UEFI) i powinien znajdować się na partycji UEFI.
