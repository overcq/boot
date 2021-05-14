# OUX/C+ OS boot loader
## Specyfikacja

Dostępne są następujace moduły:
1. program Master Boot Record (MBR)
2. program Volume Boot Record (VBR)
3. File Boot Loader (FBL)

Proces uruchamiania może przebiegać w jeden z następujących sposobów:
1. MBR ➪ VBR na pierwszej uruchamialnej partycji ➪ FBL
2. MBR ➪ FBL

Pierwszy ma zastosowanie, gdy na dysku jest tablica partycji w MBR, a na partycji jest VBR. Drugi wtedy, gdy na dysku nie ma tablicy partycji, lecz bezpośrednio po MBR jest system plików.

W VBR lub MBR od bajtu nr 224 znajduje się 5 B informacji, gdzie położony jest i jaki ma rozmiar plik FBL: najpierw w 3 B w formacie CHS (zgodnym z używanym w tablicy partycji MBR) jest zapisane położenie pliku FBL na dysku, a następnie w 2 B jest liczba sektorów o rozmiarze 512 B składająca się na rozmiar tego pliku.

<table>
<caption>Dane o położeniu i rozmiarze pliku FBL</caption>
<thead>
    <tr><th rowspan="2">224</th><th colspan="2">225</th><th rowspan="2">226</th><th rowspan="2">227–228</th></tr>
    <tr><th>7–6</th><th>5–0</th></tr>
</thead>
<tbody>
    <tr><td>head</td><td>9–8 bits of cylinder</td><td>sector</td><td>7–0 bits of cylinder</td><td>count of sectors</td></tr>
</tbody>
</table>

Liczba sektorów w MBR lub VBR równa 0 oznacza, że FBL nie ma być uruchamiany z tego modułu. Jeżeli liczba sektorów jest równa 0 w VBR, to proces uruchamiania zostanie zatrzymany.

## Uwagi

* *mbr.S* nie zawiera skopiowanej z *vbr.S* obsługi większego pliku FBL.
* *vbr.S* nie zawiera obsługi dwubajtowej liczby sektorów, a w specyfikacji znajduje się 1 bajt liczby sektorów rozmiaru FBL.
* *vbr.S* nie zawiera poprawnej obsługi odczytu wielosektorowego.
