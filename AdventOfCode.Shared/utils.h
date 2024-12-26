#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <shellapi.h>
#include <debugapi.h>
#elif unix
#include <string>
#include <format>
#endif

inline void openBrowser(const char* url)
{
#ifdef _WIN32
    ShellExecuteA(0, 0, url, 0, 0, SW_SHOW);
#elif unix
    system(std::format("xdg-open {}", url).data());
#endif
}

inline std::string durationToString(const std::chrono::steady_clock::duration duration)
{
    const char* runtimeStamps[] = { " ns", " us", " ms" };

    int type = 0;
    unsigned long long runtime = duration.count();

    if (runtime >= 100000)
    {
        runtime = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        type = 1;

        if (runtime >= 100000)
        {
            runtime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            type = 2;
        }
    }

    std::string result = std::to_string(runtime);
    result.append(runtimeStamps[type]);

    return result;
}

inline bool isDebuggerPresent()
{
#ifdef _WIN32
    return IsDebuggerPresent();
#elif unix
    // TODO
    return false;
#endif
}
