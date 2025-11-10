#pragma once
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class Logger {
private:
    fs::path     logPath;   // where the log will be written
    std::ofstream logFile;  // output stream

    // Build a timestamp string like "2025-11-09 14:33:21"
    std::string getTimestamp() const;

public:
    // Pass in a full path to a log file
    explicit Logger(const std::string& fileName);
    ~Logger();

    // Write one log line with timestamp
    void write(const std::string& message);

    // Write a separator line
    void writeDivider();
};
