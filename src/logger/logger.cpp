#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// --- Constructor ---
Logger::Logger(const string& fileName)
    : logPath(fileName), logFile(fileName, ios::app)
{
    // Ensure directory exists
    fs::create_directories(fs::path(fileName).parent_path());

    if (!logFile.is_open()) {
        cerr << "[Logger] Failed to open log file at " << logPath << '\n';
    } else {
        cout << "[Logger] Writing logs to: " << logPath << '\n';
    }
}

// --- Destructor ---
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// --- Timestamp helper ---
string Logger::getTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);

    tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now_c);
#else
    localtime_r(&now_c, &localTime);
#endif

    ostringstream oss;
    oss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// --- Logging methods ---
void Logger::write(const string& message) {
    if (logFile.is_open()) {
        logFile << "[" << getTimestamp() << "] " << message << '\n';
        logFile.flush();
    }
}

void Logger::writeDivider() {
    if (logFile.is_open()) {
        logFile << "------------------------------------------------------------\n";
    }
}
