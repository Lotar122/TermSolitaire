#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <random>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <string>

#include "Classes/Display/Display.hpp"
#include "Classes/Input/Input.hpp"
#include "Classes/Card/Card.hpp"
#include "Classes/Logic/Logic.hpp"

#include "Structs/BytePair.hpp"

static const char32_t* gameWin = UR"(┌──────────────────────────────────────────────────────────────────────────────┐
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                  __   __           __        __          _                   |
|                  \ \ / /__  _   _  \ \      / /__  _ __ | |                  |
|                   \ V / _ \| | | |  \ \ /\ / / _ \| '_ \| |                  |
|                    | | (_) | |_| |   \ V  V / (_) | | | |_|                  |
|                    |_|\___/ \__,_|    \_/\_/ \___/|_| |_(_)                  |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
|                                                                              |
└──────────────────────────────────────────────────────────────────────────────┘
)";

enum class SidePileEnum : uint8_t
{
    Draw = 0,
    
    ePile = 1,
    aPile = 2,
    bPile = 3,
    cPile = 4,
    dPile = 5,

    E = 1,
    A = 2,
    B = 3,
    C = 4,
    D = 5
};

class Game
{
    Display display;
    Input input;

    std::vector<Card> deck;
    std::vector<Card*> shuffledDeck;
    std::vector<std::vector<Card*>> mainPiles = { {}, {}, {}, {}, {}, {}, {} };
    std::vector<std::vector<Card*>> sidePiles = { {}, {}, {}, {}, {}, {} };
    std::vector<Card*>* drawStack = &sidePiles[0];

    std::random_device randomDevice;
    std::mt19937 randomEngine;

    uint16_t score = 0, moves = 0;
public:
    bool stop = false;

    std::unordered_map<BytePair, char32_t*> cardSpriteManager;

    Game();

    int update();
};