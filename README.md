# Przenośna implementacja algorytmu kwantyzacji wierzchołków trójkątów z wykorzystaniem interfejsu Kokkos
## Bogumił Sokołowski-Duda
### Uruchamianie pliku *execute*

Podczas uruchamiania pliku należy podać następujące argumenty:

 - *\<NAZWA PLIKU\>* - musi zostać podana jako pierwszy argument, plik musi znajdować się w ścieżce '../model_files'
 - -t:*\<ILOŚĆ WĄTKÓW\>* - wartość domyślna 1
 - -s: *\<SKALA\>* - wartość domyślna 1
 - --log - algorytm utworzy plik '*.log*' oraz doda wpis do pliku '*results.csv*'
 - --d lub --debug - algorytm będzie wyświetlał komunikaty debugowe (nie zaleca się używania razem z flagą '*--log*' gdyż może to zniekształcać wyniki)
 - --help - algorytm wyświetli komunikat informacyjny
