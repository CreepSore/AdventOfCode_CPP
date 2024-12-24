#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <shellapi.h>
#elif unix
#include <string>
#include <format>
#endif

inline void openBrowser(const char* url)
{
#ifdef _WIN32
    ShellExecuteA(0, 0, url, 0, 0, SW_SHOW);
#elif unix
    system(std::format("xdg-open {}").data());
#endif
}
