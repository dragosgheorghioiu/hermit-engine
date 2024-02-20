#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString() {
  std::time_t time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  char timeString[std::size("yyyy-mmm-dd hh:mm:ss")];
  std::strftime(std::data(timeString), std::size(timeString), "%d-%b-%Y %T",
                std::localtime(&time));
  return timeString;
}

void Logger::Log(const std::string &message) {
  std::string timeString = CurrentDateTimeToString();

  std::cout << "\033[1;32m[ LOG | " << timeString << " ] - " << message
            << "\033[1;0m" << std::endl;

  LogEntry logEntry;
  logEntry.message = message;
  logEntry.type = LOG_INFO;
  messages.push_back(logEntry);
}

void Logger::Err(const std::string &message) {
  std::string timeString = CurrentDateTimeToString();

  std::cout << "\033[1;31m[ ERR | " << timeString << " ] - " << message
            << "\033[1;0m" << std::endl;

  LogEntry logEntry;
  logEntry.message = message;
  logEntry.type = LOG_ERROR;
  messages.push_back(logEntry);
}
