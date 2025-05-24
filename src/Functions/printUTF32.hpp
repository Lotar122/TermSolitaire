#pragma once

#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <string>

static std::string UTF32ToUTF8(const char32_t* ptr, size_t size)
{
    std::string out;
    out.reserve(size * sizeof(char32_t));

    for (int i = 0; i < size; i++) 
    {
        char32_t cp = ptr[i];
        if (cp <= 0x7F) 
        {
            out.push_back(static_cast<char>(cp));
        } 
        else if (cp <= 0x7FF) 
        {
            out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } 
        else if (cp <= 0xFFFF) 
        {
            out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } 
        else 
        {
            out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }

    out.shrink_to_fit();

    return out;
}

static inline void printUTF32(const char32_t* str, size_t size)
{
    std::string utf8 = UTF32ToUTF8(str, size);
    fputs(utf8.c_str(), stdout);
}