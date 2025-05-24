#pragma once

#include <string>
#include <iostream>

class Input
{
    std::string inputBuffer;
public:
    void readInput()
    {
        std::getline(std::cin, inputBuffer);
    }

    inline const std::string& _inputBuffer() const { return inputBuffer; };
};