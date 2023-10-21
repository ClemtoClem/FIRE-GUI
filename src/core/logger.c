#include "logger.h"

bool isDebug = false;
FILE *logFile = NULL;

void openLogFile()
{
	logFile = fopen("log.txt", "w+");
	if (logFile != NULL) {
		fprintf(logFile, "+------------------------+\n"
						 "|       Separator        |\n"
						 "+------------------------+\n\n");
	}
}

void debugLog(bool debug)
{
	isDebug = debug;
	if (debug == true)
		printf("+------------------------+\n"
			   "|       DEBUG MODE       |\n"
			   "+------------------------+\n\n");
}

void Log_fprinf(TypeLog type, const char *format, ...)
{
	if (logFile != NULL) {
		char buffer[500];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);

		/* affichage du temps avec le message */
		time_t sec;
		time(&sec);
		struct tm instant = *localtime(&sec);

		fprintf(logFile, "%d-%d-%d %d:%d:%d ",
			instant.tm_year, instant.tm_mday+1, instant.tm_mon+1,
			instant.tm_hour, instant.tm_min, instant.tm_sec);

		switch(type) {
			case LOG_DEBUG:		fprintf(logFile, "[  DEBUG  ] : %s\n", buffer); break;
			case LOG_IMPORTANT: fprintf(logFile, "[IMPORTANT] : %s\n", buffer); break;
			case LOG_SUCCESS:	fprintf(logFile, "[ SUCCESS ] : %s\n", buffer); break;
			case LOG_WARNING:	fprintf(logFile, "[ WARNING ] : %s\n", buffer); break;
			case LOG_ERROR:		fprintf(logFile, "[  LOG_ERROR  ] : %s\n", buffer); break;
			default: break;
		}
	}
}

void Log_prinf(TypeLog type, const char *format, ...)
{
	if (isDebug == true) {
		char buffer[500];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		switch(type) {
			case LOG_DEBUG: printf("DEBUG : %s\n", buffer); break;
			case LOG_IMPORTANT: printf("\e[1;94mIMPORTANT : %s\e[0m\n", buffer); break;
			case LOG_SUCCESS: printf("\e[1;32mSUCCESS : %s\e[0m\n", buffer); break;
			case LOG_WARNING: printf("\e[7;33mWARNING : \e[1;33m%s\e[0m\n", buffer); break;
			case LOG_ERROR: printf("\e[7;31m LOG_ERROR : \e[1;31m%s\e[0m\n", buffer); break;
			default: break;
		}
		va_end(args);
	}
}

void closeLogFile()
{
	fprintf(logFile, "\n");
	fclose(logFile);
}
