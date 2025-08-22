
#pragma once

#ifdef _WIN32
    #include <windows.h>
    inline void sleep_ms(int ms) { Sleep(ms); }
#else
    #include <thread>
    #include <chrono>
    inline void sleep_ms(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
#endif
