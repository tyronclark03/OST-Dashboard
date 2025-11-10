/*
 * Module: Scanner
 * Version: v0.4.0
 * Description:
 *   - Resolve project root at runtime (works from /bin or root)
 *   - Scan tests/mock_data for .ost/.OST
 *   - Convert size to GB
 *   - Classify by thresholds
 *   - Log results to timestamped file in /logs
 */

#include "scanner.hpp"
#include "../logger/logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// ------------------------------------------------------------
// Decide which category the file belongs to
// (these can later move to config)
// ------------------------------------------------------------
std::string classifyFileSize(double sizeGB) {
    // > 30 GB is critical
    if (sizeGB > 30.0) {
        return "CRITICAL";
    }
    // 20–30 GB is just a warning for now
    if (sizeGB >= 20.0) {
        return "WARNING";
    }
    // under 20 GB we don't need to show in console
    return "";
}

// ------------------------------------------------------------
// Walk the directory and collect file info
// ------------------------------------------------------------
std::vector<FileRecord> listFiles(const fs::path& dirPath) {
    std::vector<FileRecord> results;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(
                 dirPath,
                 fs::directory_options::skip_permission_denied))
        {
            if (!entry.is_regular_file()) {
                continue;
            }

            const auto ext = entry.path().extension().string();
            if (ext != ".ost" && ext != ".OST") {
                continue;
            }

            FileRecord rec;
            rec.name = entry.path().filename().string();
            rec.path = entry.path().string();

            // size in bytes → GB
            double sizeBytes = static_cast<double>(fs::file_size(entry.path()));
            double sizeGB    = sizeBytes / (1024.0 * 1024.0 * 1024.0);

            rec.sizeGB  = sizeGB;
            rec.category = classifyFileSize(sizeGB);

            results.push_back(rec);
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "[Scanner] filesystem error: " << e.what() << '\n';
    }

    return results;
}

// ------------------------------------------------------------
// main entry
// ------------------------------------------------------------
int main() {
    try {
        cout << "=== OST Scanner v0.4.0 ===\n";

        // figure out where we are running from (root or /bin)
        fs::path execPath = fs::current_path();
        fs::path rootPath = execPath;

        // if we're in /bin, go up one level to project root
        if (execPath.filename() == "bin") {
            rootPath = execPath.parent_path();
        }

        // build path to mock data
        fs::path mockDataPath = rootPath / "tests" / "mock_data";

        // build a per-run log file name
        std::time_t now = std::time(nullptr);
        std::tm local{};
#ifdef _WIN32
        localtime_s(&local, &now);
#else
        localtime_r(&now, &local);
#endif

        std::ostringstream logName;
        logName << "scan_report_"
                << (1900 + local.tm_year) << "-"
                << std::setw(2) << std::setfill('0') << (local.tm_mon + 1) << "-"
                << std::setw(2) << std::setfill('0') << local.tm_mday
                << ".txt";

        fs::path logsDir = rootPath / "logs";
        fs::create_directories(logsDir);
        fs::path logFilePath = logsDir / logName.str();

        // init logger with full path
        Logger logger(logFilePath.string());

        cout << "Scanning Directory: " << mockDataPath << "\n\n";
        logger.write("=== New Scan Started ===");
        logger.write("Scanning Directory: " + mockDataPath.string());
        logger.writeDivider();

        // run scanner
        std::vector<FileRecord> files = listFiles(mockDataPath);

        // header for console
        cout << left << setw(26) << "File"
             << setw(12) << "Size(GB)"
             << "Category\n";
        cout << string(60, '-') << '\n';

        int flagged = 0;
        for (const auto& f : files) {
            // don't show normal files in console (but still log them)
            if (f.category.empty()) {
                // write info to log for audit
                logger.write(f.name + " | " + f.path + " | " + to_string(f.sizeGB) + " GB | NORMAL");
                continue;
            }

            // console output
            cout << left << setw(26) << f.name
                 << setw(12) << fixed << setprecision(2) << f.sizeGB
                 << f.category << '\n';

            // logger output
            std::ostringstream oss;
            oss << f.name << " | " << f.path << " | "
                << std::fixed << std::setprecision(2) << f.sizeGB << " GB | "
                << f.category;
            logger.write(oss.str());

            flagged++;
        }

        // summary
        cout << "\n=== Scan Summary ===\n";
        cout << "Total Files Scanned : " << files.size() << '\n';
        cout << "Files Exceeding Limit: " << flagged << '\n';

        logger.writeDivider();
        logger.write("Total Files Scanned : " + std::to_string(files.size()));
        logger.write("Files Exceeding Limit: " + std::to_string(flagged));
        logger.write("=== Scan Complete ===");

    } catch (const std::exception& e) {
        std::cerr << "[Scanner] unexpected error: " << e.what() << '\n';
    }

    return 0;
}
