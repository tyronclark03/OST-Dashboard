/*
 * Module: Scanner
 * Version: v0.4.0
 * Description:
 *  Traverses mock data directory, gathers metadata, and classifies files by size thresholds.
 *  Adds dynamic root path resolution and per-run log rotation.
 * Last update: 2025-11-09
 */

#include "scanner.hpp"
#include "../logger/logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <vector>
#include <ctime>
#include <filesystem>

//--------------------------------------
// File classification helper
//--------------------------------------
string classifyFileSize(double sizeGB)
{
    if (sizeGB > 30.0)
        return "CRITICAL";
    else if (sizeGB >= 20.0)
        return "WARNING";
    else
        return "";
}

//--------------------------------------
// File scanning logic
//--------------------------------------
vector<FileRecord> listFiles(const fs::path& dirPath)
{
    vector<FileRecord> fileList;

    try
    {
        for (const auto& entry :
             fs::recursive_directory_iterator(dirPath, fs::directory_options::skip_permission_denied))
        {
            if (entry.is_regular_file())
            {
                auto ext = entry.path().extension().string();
                if (ext == ".ost" || ext == ".OST")
                {
                    FileRecord record;
                    record.name = entry.path().filename().string();
                    record.path = entry.path().string();
                    double size = static_cast<double>(fs::file_size(entry.path())) /
                                  (1024.0 * 1024.0 * 1024.0); // bytes → GB
                    record.fileSize = size;
                    record.unit = "GB";
                    record.category = classifyFileSize(size);
                    fileList.push_back(record);
                }
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        cerr << "[Error] " << e.what() << '\n';
    }

    return fileList;
}

//--------------------------------------
// Entry point
//--------------------------------------
int main()
{
    try
    {
        cout << "=== OST Scanner ===\n";

        //--------------------------------------
        // Resolve project root
        //--------------------------------------
        fs::path execPath = fs::current_path();
        fs::path rootPath = execPath;

        if (execPath.filename() == "bin")
            rootPath = execPath.parent_path();

        fs::path mockDataPath = rootPath / "tests" / "mock_data";

        //--------------------------------------
        // Timestamped log rotation
        //--------------------------------------
        time_t now = time(nullptr);
        tm* local = localtime(&now);
        ostringstream logFileName;
        logFileName << "scan_report_"
                    << (1900 + local->tm_year) << "-"
                    << setw(2) << setfill('0') << (1 + local->tm_mon) << "-"
                    << setw(2) << setfill('0') << local->tm_mday << ".txt";

        fs::path logsDir = rootPath / "logs";
        fs::create_directories(logsDir);

        fs::path logFilePath = logsDir / logFileName.str();
        Logger logger(logFilePath.string());

        //--------------------------------------
        // Scan operation
        //--------------------------------------
        cout << "Scanning Directory: " << mockDataPath << "\n\n";
        logger.write("=== New Scan Started ===");
        logger.write("Scanning Directory: " + mockDataPath.string());
        logger.writeDivider();

        vector<FileRecord> results = listFiles(mockDataPath);

        cout << left << setw(25) << "File"
             << setw(12) << "Size(GB)"
             << "Category\n";
        cout << string(60, '-') << '\n';

        int flaggedCount = 0;
        for (const auto& file : results)
        {
            if (file.category.empty())
                continue;

            cout << left << setw(25) << file.name
                 << setw(12) << fixed << setprecision(2) << file.fileSize
                 << file.category << '\n';

            ostringstream oss;
            oss << file.name << " | "
                << fixed << setprecision(2) << file.fileSize << " GB | "
                << file.category;
            logger.write(oss.str());
            flaggedCount++;
        }

        //--------------------------------------
        // Summary
        //--------------------------------------
        cout << "\n=== Scan Summary ===\n";
        cout << "Total Files Found: " << results.size() << '\n';
        cout << "Files Exceeding Limit: " << flaggedCount << '\n';

        logger.writeDivider();
        logger.write("Total Files Scanned: " + to_string(results.size()));
        logger.write("Files Exceeding Limit: " + to_string(flaggedCount));
        logger.write("=== Scan Complete ===\n");
    }
    catch (const fs::filesystem_error& e)
    {
        cerr << "[Error] " << e.what() << '\n';
    }
    catch (const exception& e)
    {
        cerr << "[Error] " << e.what() << '\n';
    }

    return 0;
}
