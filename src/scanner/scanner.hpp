#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Simple container for each OST file
struct FileRecord {
    std::string name;       // filename 
    std::string path;       // full path to file
    double      sizeGB{0.0}; // file size
    std::string unit{"GB"};
    std::string category;   // "", "WARNING", "CRITICAL"
};

// Decide what label to give based on size
std::string classifyFileSize(double sizeGB);

// Walk the directory and return all matching ost files
std::vector<FileRecord> listFiles(const fs::path& dirPath);
