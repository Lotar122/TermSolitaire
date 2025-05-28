#pragma once

#include <string>
#include <iostream>
#include <ranges>

class Input
{
    std::string inputBuffer;
public:
    inline void readInput()
    {
        std::getline(std::cin, inputBuffer);
        std::ranges::transform(inputBuffer, inputBuffer.begin(), [](char c) { return std::tolower(c); });
    }

    inline std::string& _inputBuffer() { return inputBuffer; };
};