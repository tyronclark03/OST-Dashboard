#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

Logger::Logger(const std::string& fileName)
    : logPath(fileName),
      logFile(fileName, std::ios::app)
{
    // make sure the directory for the log exists
    fs::create_directories(logPath.parent_path());

    // basic runtime feedback
    if (!logFile.is_open()) {
        std::cerr << "[Logger] Failed to open log file at: " << logPath << '\n';
    } else {
        std::cout << "[Logger] Writing logs to: " << logPath << '\n';
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

std::string Logger::getTimestamp() const {
    // grab current time
    auto now   = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#ifdef _WIN32
    // windows secure version
    localtime_s(&localTime, &now_c);
#else
    // posix / linux / mac version
    localtime_r(&now_c, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::write(const std::string& message) {
    if (logFile.is_open()) {
        logFile << "[" << getTimestamp() << "] " << message << '\n';
        logFile.flush(); // make sure it hits disk
    }
}

void Logger::writeDivider() {
    if (logFile.is_open()) {
        logFile << "------------------------------------------------------------\n";
        logFile.flush();
    }
}
