/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#if 0
    void LoggerInit();
    void debugLogger(const char *fmt...);
    extern "C" void Logger(const char *fmt...);
#else
    #define LoggerInit() {}
    #define Logger(...) {}
#endif

