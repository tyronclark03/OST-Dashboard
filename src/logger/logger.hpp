#pragma once
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

namespace fs = filesystem;


class Logger {
private:
    fs::path logPath; 
    ofstream logFile;

public:
    explicit Logger(const string& fileName);
    ~Logger();

    void write(const string& message);
    void writeDivider();

private:
    string getTimestamp() const;
};
