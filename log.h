#ifndef __WINDNET_LOG_H__
#define __WINDNET_LOG_H__

namespace Windnet {

#define LOG_TRACE  0
#define LOG_DEBUG  1
#define LOG_INFO   2
#define LOG_WARN   3
#define LOG_ERROR  4
#define LOG_FATAL  5

#define writeLog(level, log) flushToLog(level, log, __FILE__, __LINE__)


}

#endif
