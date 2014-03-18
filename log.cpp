#include "log.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int g_logLevel;

namespace Windnet {
const char *log_name[] = {"LOG_TRACE", "LOG_DEBUG", "LOG_INFO", "LOG_WARN", "LOG_ERROR", "LOG_FATAL"};

void flushTolog(int level, const char *log, const char *filename, int line) {
	if (level < g_logLevel) {
		return;
    }
	time_t now = time(0);
    struct tm t;
    localtime_r(&now, &t);

	char buffer[1024];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", &t);
	//snprintf(buffer, 1024, "%s %s", log_name[level], buffer);
	sprintf(buffer, "%s %s %s %s %d\n", buffer, log_name[level], log, filename, line);
	write(STDOUT_FILENO, buffer, strlen(buffer));
    assert(level != LOG_FATAL);
}

void logger(const char *filename, int line, const char *format, ...) {
	time_t t = time(0);
	struct tm tm;
    localtime_r(&t, &tm);
    char buf[1024] = {0};
    strftime(buf, 32, "%Y-%m-%d %H:%M:%S  ", &tm);

    va_list arg_ptr;
    va_start(arg_ptr, format);

    char buf2[1024];
    vsprintf(buf2, format, arg_ptr);
    strcat(buf, buf2);
    sprintf(buf, "%s  %s:%d\n", buf, filename, line);
    write(1, buf, sizeof(buf));
    va_end(arg_ptr);
}
}
