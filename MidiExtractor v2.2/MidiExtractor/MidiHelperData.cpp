#include <stdio.h> // printf
#include <stdint.h>
#include <stdlib.h> // malloc, free
#include <stdarg.h>
#include <iostream>
#include <string>
#include "MidiHelperData.h"



/**
 * Default log function which prints to the stderr.
 * @param level Log level
 * @param message Log message
 * @param data User supplied data (not used)
 */
void ct_default_log_function(int level, const char* message)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
    WORD wOldColorAttrs = csbiInfo.wAttributes;

    switch (level)
    {
    case CT_PANIC:
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << "panic: " << message << std::endl;
        break;

    case CT_ERR:
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "error: " << message << std::endl;
        break;

    case CT_WARN:
#ifdef _DEBUG
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "warning: " << message << std::endl;
#endif // #ifdef _DEBUG
        break;

    case CT_INFO:
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << "" << message << std::endl;
        break;

    case CT_DBG:
#ifdef _DEBUG
        std::cout << "debug: " << message << std::endl ;
#endif // #ifdef _DEBUG
        break;

    default:
#ifdef _DEBUG
        std::cout << message << std::endl ;
#endif // #ifdef _DEBUG
        break;
    }


    // Restore the original text colors. 
    SetConsoleTextAttribute(hStdout, wOldColorAttrs);
}


/**
 * Print a message to the log.
 * @param level Log level (#ct_log_level).
 * @param fmt Printf style format string for log message
 * @param ... Arguments for printf 'fmt' message string
 * @return Always returns #CT_FAILED
 */
int ct_log(int level, const char* fmt, ...)
{
    if ((level >= 0) && (level < CT_LAST_LOG_LEVEL))
    {
        char errbuf[1024];

        va_list args;
        va_start(args, fmt);
        CT_VSNPRINTF(errbuf, sizeof(errbuf), fmt, args);
        va_end(args);

        ct_default_log_function(level, errbuf);
    }

    return CT_FAILED;
}
