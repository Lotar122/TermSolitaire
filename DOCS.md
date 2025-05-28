# 📖 Dokumentacja
## **Koncepty (C++20)**
|Nazwa | Opis|
|-|-|
|Numeric | Wszelkie wartości numeryczne.|
|Integer | Wszelkie typy liczb całkowitych.|
|StdVector | Koncept Wektoru z biblioteki standardowej C++.|
|Callable | Wszelkie Obiekty które można wywołać jak funkcję.|

## **Funkcje**
|Nazwa | Opis|
|-|-|
|printUTF32 | Funkcja służąca do wypisania łańcucha znaków UTF32 na ```stdout```.|
|moveLastElement | Przenosi ostatni element z Wektoru 1 do Wektoru 2.|

## **Struktury**
|Nazwa | Opis|
|-|-|
|BytePair | Typ danych do przechowywania dwóch bajtów jako pary (```unsigned short```)|
|BytePairDecoded | Struktura do przechowywania dwóch bajtów jako rozdzieloną parę|

## **Klasy**
|Nazwa | Opis|
|-|-|
|Card | Reprentuję dowolną kartę.|
|Display | Odpowiedzialna za wyświetlanie treści.|
|Game | Enkapsuluje wszystkie aspekty gry i udostępnia jedną metodę ```update```, która powinna być wywoływana w pętli gry.|
|Input | Klasa odpowiedzialna za czytanie komend z ```stdin```.|
|Logic | Enkapsuluje całą logikę gry, udostępnia jedną metodę ```handle```, której wywołanie następuje w klasie ```Game```.|