# OUX/C+ OS boot loader – instalacja

W celu zainstalowania programu

1. w pliku dysku do uruchomienia w symulatorze *bochs* lub
2. na dysku usb do uruchomienia na rzeczywistej maszynie

należy zainstalować ze źródeł

1. *OUX/C+ programming language modules and build environment* z https://github.com/overcq/oux wraz z dwoma programami będącymi w tym projekcie: *filesystem/mkfs* oraz *filesystem/mount*.
2. *OUX/C+ OS boot loader* z https://github.com/overcq/boot

## Instalacja wymaganych części *OUX/C+ programming language modules and build environment*

Potrzeba sklonować źródła poleceniem `git clone https://github.com/overcq/oux.git`. Następnie przejść do pobranego katalogu i zbudować środowisko poleceniem `cd oux && make build`. Wtedy przejść na użytkownika *root* poleceniem `su` i zainstalować biblioteki środowiska OUX/C+ poleceniem `make install`.

Wymagane programy można zainstalować w następujący sposób. Wyjść z użytkownika *root*, przejść do katalogu *oux/program/filesystem/mkfs* i zbudować program poleceniem `cd program/filesystem/mkfs && make build`. Przejść do katalogu *oux/program/filesystem/mount* i zbudować program poleceniem `cd ../mount && make build`. Przejść na użytkownika *root* poleceniem `su` i zainstalować poszczególne programy poleceniem `make install && cd ../mkfs && make install`. A na końcu wyjść z użytkownika *root*.

## Instalacja *OUX/C+ OS boot loader*

Potrzeba sklonować źródła poleceniem `git clone https://github.com/overcq/boot.git`. Następnie przejść do pobranego katalogu i zbudować program poleceniem `cd boot && make build`. Wtedy przejść na użytkownika *root* poleceniem `su` i zainstalować program:

1. w pliku poleceniem `make install` lub
2. na dysku USB poleceniem `make usb`

A na końcu wyjść z użytkownika *root*.

W pierwszym przypadku wystarczy zainstalować i uruchomić symulator *bochs*, będąc w katalogu *boot*. W drugim przypadku można uruchomić komputer z tego dysku USB.
