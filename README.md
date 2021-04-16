# OUX/C+ OS boot loader
## Specyfikacja

Dostępne są następujace moduły:
1. program Master Boot Record (MBR)
2. program Volume Boot Record (VBR)
3. File Boot Loader

Proces uruchamiania może przebiegać w jeden z następujących sposobów:
1. MBR ➪ VBR na pierwszej uruchamialnej partycji ➪ File Boot Loader
2. MBR ➪ File Boot Loader

Pierwszy ma zastosowanie, gdy na dysku jest tablica partycji w MBR, a na partycji jest VBR. Drugi wtedy, gdy na dysku nie ma tablicy partycji, lecz bezpośrednio po MBR jest system plików.

W VBR lub MBR od bajtu nr 224 znajdują się 4 B informacji, gdzie znajduje się i jaki ma rozmiar plik File Boot Loadera: najpierw w 3 B w formacie CHS (zgodnym z używanym w tablicy partycji MBR) jest zapisane położenie pliku File Boot Loadera na dysku, a następnie w 1 B jest liczba sektorów o rozmiarze 512 B składająca się na rozmiar tego pliku.

<table>
<caption>Dane o położeniu i rozmiarze pliku File Boot Loadera</caption>
<thead>
    <tr><th rowspan="2">224</th><th colspan="2">225</th><th rowspan="2">226</th><th rowspan="2">227</th></tr>
    <tr><th>7–6</th><th>5–0</th></tr>
</thead>
<tbody>
    <tr><td>head</td><td>9–8 bits of cylinder</td><td>sector</td><td>7–0 bits of cylinder</td><td>count of sectors</td></tr>
</tbody>
</table>

Liczba sektorów w MBR lub VBR równa 0 oznacza, że File Boot Loader nie ma być uruchamiany z tego modułu. Jeżeli liczba sektorów jest równa 0 w VBR, to proces uruchamiania zostanie zatrzymany.
