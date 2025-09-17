#include "sep_log.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>



/**
 * Prints a log message with the given level and format string.
 *
 * @param level The level of the log message. Can be one of the following:
 *             - INFO: Informational message
 *             - WARNING: Warning message
 *             - ERROR: Error message
 * @param format The format string of the log message.
 * @param ... Variable number of arguments to be used in the format string.
 *
 * This function computes the current timestamp and formats the log message
 * string using the given format string and arguments. It then prints the
 * log message to the standard output with the corresponding level.
 */
void s_log(int level, const char* format, ...){
    
    time_t current_time;
    char* time_string;
    struct tm* time_info;
    char timestamp[20];
    char buffer[1024];

    // compute the the current timestamp
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    //format message string
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    

    switch (level)
    {
    case INFO:
        printf("%s - [INFO] - %s\n", timestamp,buffer);
        break;
    case WARNING:
        printf("%s - [WARNING] - %s\n",timestamp,buffer);
        break;
    case ERROR:
        printf("%s - [ERROR] - %s\n", timestamp,buffer);
        break;
    default:
        break;
    }
}