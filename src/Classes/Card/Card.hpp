#pragma once

#include <cstdint>
#include <cstddef>

constexpr size_t cardRankEnumLenght = 13;
enum class CardRank : uint8_t
{
    Ace = 0,
    Two = 2,
    Three = 4,
    Four = 6,
    Five = 8,
    Six = 10,
    Seven = 12,
    Eight = 14,
    Nine = 16,
    Ten = 18,
    Jack = 20,
    Queen = 22,
    King = 24,
};

constexpr size_t cardColorEnumLenght = 4;
enum class CardColor : uint8_t
{
    Spades = 0, //Black, ♠
    Clubs = 1, //Black, ♣
    Hearts = 2, //Red, ♥
    Diamonds = 3, //Red, ♦
};

constexpr size_t randSymbolsSize = 26;
static const char32_t* rankSymbols[26] = {
    U"A ", U" A",
	U"2 ", U" 2",
	U"3 ", U" 3",
	U"4 ", U" 4",
	U"5 ", U" 5",
	U"6 ", U" 6",
	U"7 ", U" 7",
	U"8 ", U" 8",
	U"9 ", U" 9",
	U"10", U"10",

	U"J ", U" J",
	U"Q ", U" Q",
	U"K ", U" K"
};

constexpr size_t colorSymbolsSize = 4;
static const char32_t colorSymbols[4] = {
	U'♠', U'♣', U'♥', U'♦'
};

static const char32_t* cardMap = 
U"┌───────┐"
U"│XX Y   │"
U"│       │"
U"│   Y XX│"
U"└───────┘";
static const char32_t* cardBackSprite = 
U"┌───────┐"
U"|#######|"
U"|#######|"
U"|#######|"
U"└───────┘";

constexpr size_t cardWidth = 9;
constexpr size_t cardHeight = 5;
constexpr size_t cardSize = cardWidth * cardHeight;

class Game;

class Card
{
    CardColor color;
    CardRank rank;

    char32_t* cardSprite = nullptr;
public:
    bool up = false;

    Card(Game* game, CardColor _color, CardRank _rank, bool _up = false);

    Card() = delete;

    char32_t getSpriteChar(size_t x, size_t y)
    {
        return up ? cardSprite[y * cardWidth + x] : cardBackSprite[y * cardWidth + x];
    }
};
