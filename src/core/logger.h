#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

extern bool isDebug;
extern FILE *logFile;

typedef enum {
	LOG_DEBUG, LOG_IMPORTANT, LOG_SUCCESS, LOG_WARNING, LOG_ERROR
} TypeLog;

#define DEBUG(format, ...) Log_prinf(LOG_DEBUG, format, ##__VA_ARGS__)
#define IMPORTANT(format, ...) Log_prinf(LOG_IMPORTANT, format, ##__VA_ARGS__)
#define SUCCESS(format, ...) Log_prinf(LOG_SUCCESS, format, ##__VA_ARGS__)
#define WARNING(format, ...) Log_prinf(LOG_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Log_prinf(LOG_ERROR, format, ##__VA_ARGS__)

void openLogFile();
void debugLog(bool debug);

void Log_fprinf(TypeLog type, const char *format, ...);
void Log_prinf(TypeLog type, const char *format, ...);

void closeLogFile();

#endif