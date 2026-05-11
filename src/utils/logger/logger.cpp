#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>

Logger global_logger;

Logger::Logger() {}

void Logger::info(const std::string& message, const char* file) {
  log("INFO", message, file);
}

void Logger::warn(const std::string& message, const char* file) {
  log("WARN", message, file);
}

void Logger::error(const std::string& message, const char* file) {
  log("ERROR", message, file);
}

void Logger::log(
    const std::string& level, 
    const std::string& message, 
    const char* file
    ) {
  std::string time = getCurrentTime();
  
  pid_t pid = getpid();

  std::string filename = file;
  size_t pos = filename.find_last_of("/\\");
  if (pos != std::string::npos) {
    filename = filename.substr(pos + 1);
  }
  std::string color;
  if (level == "INFO") color = "\033[1;32m";
  else if (level == "WARN") color = "\033[1;33m";
  else if (level == "ERROR") color = "\033[1;31m";
  
  std::string reset = "\033[0m";

  std::string paddedLevel = level;
  while(paddedLevel.length() < 5) paddedLevel += " ";

  std::ostream& out = (level == "ERROR") ? std::cerr : std::cout;
  out << color << "[" << paddedLevel << "] " << reset
    << " " << time
    << " | pid: " << pid
    << " | [" << filename << "]"
    << " | " << message
    << "\n";
}
std::string Logger::getCurrentTime() {
  using namespace std::chrono;

  auto now = system_clock::now();
  auto seconds = time_point_cast<std::chrono::seconds>(now);
  auto ms = duration_cast<milliseconds>(now - seconds).count();

  std::time_t now_c = system_clock::to_time_t(seconds);
  std::tm* parts = std::localtime(&now_c);

  std::ostringstream oss;
  oss << std::put_time(parts, "%Y-%m-%d %H:%M:%S")
      << "." << std::setw(3) << std::setfill('0') << ms;

  return oss.str();
}
