#pragma once

#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <string>

#ifdef Windows
#include <Windows.h>
#endif

static std::wstring UTF32ToUTF16(const char32_t* ptr, size_t size)
{
    std::wstring out;
    out.reserve(size * sizeof(char32_t) / sizeof(wchar_t));

    for (size_t i = 0; i < size; ++i) 
    {
        char32_t cp = ptr[i];
        if (cp <= 0xFFFF) 
        {
            out.push_back(static_cast<wchar_t>(cp));
        }
        else 
        {
            cp -= 0x10000;
            wchar_t high = static_cast<wchar_t>((cp >> 10) + 0xD800);
            wchar_t low = static_cast<wchar_t>((cp & 0x3FF) + 0xDC00);
            out.push_back(high);
            out.push_back(low);
        }
    }

    out.shrink_to_fit();
    return out;
}

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
    #ifdef Windows
    std::wstring utf16 = UTF32ToUTF16(str, size);

    // Get console handle for stdout
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        // fallback to fputs if console handle invalid
        std::string utf8 = UTF32ToUTF8(str, size);
        fputs(utf8.c_str(), stdout);
        return;
    }

    DWORD written = 0;
    BOOL result = WriteConsoleW(hConsole, utf16.data(), (DWORD)utf16.size(), &written, nullptr);
    if (!result) {
        DWORD err = GetLastError();
        fprintf(stderr, "WriteConsoleW failed with error %lu\n", err);
    }
    #else
    std::string utf8 = UTF32ToUTF8(str, size);
    fputs(utf8.c_str(), stdout);
    #endif
}