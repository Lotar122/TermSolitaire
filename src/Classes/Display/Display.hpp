﻿#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstdio>
#include <string>

#include "Classes/Card/Card.hpp"

#include "Concepts/Integer.hpp"
#include "Concepts/Numeric.hpp"
#include "Concepts/Callable.hpp"
#include "Concepts/StdVector.hpp"

#include "Functions/printUTF32.hpp"

static const char32_t* screenMap = 
UR"(┌───────────────────────────────────────────────────────────────────┐~~~~~~~~~~~
|                                                                   |~~~~~~~~~~~
└───────────────────────────────────────────────────────────────────┘~~~~~~~~~~~
~~~~1~~~~~~~~~2~~~~~~~~~3~~~~~~~~~4~~~~~~~~~5~~~~~~~~~6~~~~~~~~~7~~~~~~~~~~~~~~~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~L@@@@@@@@D
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@r
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@a
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@w
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~~~~~~~~~~~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~N@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@E
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~~~~~~~~~~~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~U@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@A
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~V@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@B
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~W@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@C
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~X@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@D
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~@@@@@@@@@~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~
H@@@@@@@@~I@@@@@@@@~J@@@@@@@@~K@@@@@@@@~Y@@@@@@@@~F@@@@@@@@~G@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~~~~~~~~~~~
@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~@@@@@@@@@~~~~~~~~~~~
)";

constexpr size_t screenMapWidth = 81;
constexpr size_t screenMapHeight = 45;
constexpr size_t screenMapSize = screenMapWidth * screenMapHeight;

static inline void clearScreen() { 
	fputs("\033[H", stdout);
};

class Display
{
    char32_t* screenDataLump = nullptr;
    char32_t** screen = nullptr;

    static constexpr size_t mainPilesCount = 7;
    uint8_t mainPileCounters[7] = { 0, 0, 0, 0, 0, 0, 0 };
    const char32_t mainPileScreenMapSymbols[7] = { U'H', U'I', U'J', U'K', U'Y', U'F', U'G' };

    static constexpr size_t sidePilesCount = 6;
    //0 - Draw, 1 - E, 2 - A, 3 - B, 4 - C, 5 - D
    const char32_t sidePileSymbols[6] = { U'L', U'N', U'U', U'V', U'W', U'X' };

    void renderCardFromPile(const std::vector<Card*>& pile, uint8_t pileID, size_t x, size_t y);
    void renderCard(char32_t symbol, Card* card, size_t x, size_t y);
public:
    Display();

    void render(
        std::vector<std::vector<Card*>>& mainPiles,
        std::vector<std::vector<Card*>>& sidePiles,
        uint16_t score, uint16_t moves
    );
};