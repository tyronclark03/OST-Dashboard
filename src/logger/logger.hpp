#pragma once
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

namespace fs = filesystem;


class Logger {
private:
    fs::path logPath;       ///< Full path to log file
    ofstream logFile;  ///< Stream for log writing

public:
    explicit Logger(const string& fileName);
    ~Logger();

    void write(const string& message);
    void writeDivider();

private:
    string getTimestamp() const;
};
