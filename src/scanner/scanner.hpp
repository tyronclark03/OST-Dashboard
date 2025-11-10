#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Simple container for each OST file
struct FileRecord {
    std::string name;       // filename only
    std::string path;       // full path to file
    double      sizeGB{0.0}; // size already converted to GB
    std::string unit{"GB"}; // always GB right now, but kept for future
    std::string category;   // "", "WARNING", "CRITICAL"
};

// Decide what label to give based on GB size
std::string classifyFileSize(double sizeGB);

// Walk the directory and return all matching .ost/.OST files
std::vector<FileRecord> listFiles(const fs::path& dirPath);
