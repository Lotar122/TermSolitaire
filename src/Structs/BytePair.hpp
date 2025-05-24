#pragma once

struct BytePairDecoded 
{
    uint8_t lower, upper;
};

using BytePair = uint16_t;

static inline BytePair encodeBytePair(uint8_t s1, uint8_t s2)
{
    return (static_cast<BytePair>(s1) << 8) | s2;
}

static inline BytePairDecoded decodeBytePair(BytePair pair)
{
    return BytePairDecoded{static_cast<uint8_t>(pair >> 8), static_cast<uint8_t>(pair & 0xFF)};
}