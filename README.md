# TermSolitaire - Pasjans w terminalu.

![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

# 📦 Instalacja
### **Wymagania**
- **CMake** (>= 3.10) (**jeżeli zamierzasz kompilować**)
- **C++20 lub nowszy**
- **Powershell (na Windowsie)**

Wypakuj projekt z pliku **.zip** i wejdz do katalogu projektu.

# 🚀 Uruchamianie
## 💻 Windows:
### **Uruchom gotowy plik wykonywalny**
W katalogu projektu (powershell): 
```sh 
.\bin\Windows\Release\TermSolitaire.exe
```
### **Skompiluj samodzielnie**
Otwórz projekt w Visual Studio i skompiluj jako projekt CMake (**niezalecane**)
## 🐧 Linux:
### **Zbuduj przy użyciu CMake (zalecany ninja)**
W katalogu projektu:
```sh
cmake -B build -G ninja
ninja -C build -j$(nproc)
```
### **Następnie uruchom**
W katalogu projektu:
```sh
./build/TermSolitaire
```

# 🕹️ Sterowanie
### **Dobranie karty**
Należy wpisać `draw`, wtedy na stosie `E` pojawi się nowa karta ze stosu rezerwowego
### **Wczesne zakończenie gry**
Należy wpisać `exit`, po czym gra zakończy się
### **Restart gry**
Należy wpisać `restart`, po czym gra rozpocznie się od nowa
### **Przełożenie kart z góry stosu X na górę stosu Y**
Należy wpisać komendę: `X > Y`, gdzie `X` i `Y` to numery bądź litery określające stos
### **Przełożenie wielu kart z końca stosu na koniec stosu**
Należy wpisać komendę: `X(Z) > Y`, gdzie `X` i `Y` to numery określające stos, `Z` to liczba kart którą chcesz przełożyć ze stosu X na stos Y

# 🎮 Przebieg gry
- Po rozpoczęciu gry można przesuwać karty wobec klasycznych zasad.
- Aby wygrać należy ułożyć wszystkie karty z danego koloru na stosach A, B, C i D. od Asa do Króla. W momencie kiedy wszystkie kart Król trafią na stosy A, B, C, i D gra zakończy się wygraną.
- Kart ze stosów A, B, C i D nie da się sciągnąć.

# 📖 Dokumentacja
### **Dokumentacja znajduję się w pliku DOCS.md**