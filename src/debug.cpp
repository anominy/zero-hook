#include "./debug.hpp"

#include <windows.h>

#include <cstdarg>
#include <cstdio>

void zero::dbg::Msg(HMODULE hModule, const char* szFormat, ...)
{
    if (!szFormat)
    {
        return;
    }
    static auto fn = reinterpret_cast<void(__cdecl*)(const char*, ...)>(GetProcAddress(hModule, "Msg"));

    char buff[1024] = {0};

    va_list args;
    va_start(args, szFormat);
    std::vsnprintf(buff, sizeof(buff), szFormat, args);
    va_end(args);

    fn("%s", buff);
}
