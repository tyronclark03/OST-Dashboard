#pragma once
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

namespace fs = filesystem;

struct FileRecord
{
    string name{};
    string path{};
    double fileSize{0.0};
    string unit{};
    string category{};
};

// --- Function declarations ---
string classifyFileSize(double sizeGB);
vector<FileRecord> listFiles(const fs::path& dirPath);
