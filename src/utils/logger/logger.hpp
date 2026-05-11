#pragma once

#include <string>

class Logger {
  public:
    Logger();

    void info(const std::string& message, const char* file);
    void warn(const std::string& message, const char* file);
    void error(const std::string& message, const char* file);

  private:
    void log(const std::string& level, const std::string& message, const char* file);
    std::string getCurrentTime();
};

// global logger declaration
extern Logger global_logger;

// macros for loggers
#ifndef LOG_INFO
#define LOG_INFO(msg) global_logger.info(msg, __FILE__)
#endif

#ifndef LOG_WARN
#define LOG_WARN(msg) global_logger.warn(msg, __FILE__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(msg) global_logger.error(msg, __FILE__)
#endif
