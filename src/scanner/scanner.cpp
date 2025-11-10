/*
 * Module: Scanner
 * Version: v0.5.0
 * Description:
 *   - Resolve project root dynamically (works from /bin or root)
 *   - Load configuration from scanner.conf
 *   - Scan for .ost/.OST files under configured mock_data_dir
 *   - Convert size to GB, categorize by config thresholds
 *   - Log results to timestamped file in configured logs_dir
 */

#include "scanner.hpp"
#include "../logger/logger.hpp"
#include "../config/config.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// ------------------------------------------------------------
// Decide which category the file belongs to
// ------------------------------------------------------------
std::string classifyFileSize(double sizeGB, double warnThreshold, double critThreshold) {
    if (sizeGB > critThreshold) {
        return "CRITICAL";
    }
    if (sizeGB >= warnThreshold) {
        return "WARNING";
    }
    return ""; // under warning threshold = NORMAL
}

// ------------------------------------------------------------
// Walk the directory and collect file info
// ------------------------------------------------------------
std::vector<FileRecord> listFiles(const fs::path& dirPath, double warnThreshold, double critThreshold) {
    std::vector<FileRecord> results;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(
                 dirPath,
                 fs::directory_options::skip_permission_denied))
        {
            if (!entry.is_regular_file()) continue;

            const auto ext = entry.path().extension().string();
            if (ext != ".ost" && ext != ".OST") continue;

            FileRecord rec;
            rec.name = entry.path().filename().string();
            rec.path = entry.path().string();

            // Convert bytes to GB
            double sizeBytes = static_cast<double>(fs::file_size(entry.path()));
            double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);

            rec.sizeGB = sizeGB;
            rec.category = classifyFileSize(sizeGB, warnThreshold, critThreshold);

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
        cout << "=== OST Scanner v0.5.0 ===\n";

        // Resolve runtime path (root vs /bin)
        fs::path execPath = fs::current_path();
        fs::path rootPath = (execPath.filename() == "bin")
                                ? execPath.parent_path()
                                : execPath;

        // ------------------------------------------------------------
        // Load configuration
        // ------------------------------------------------------------
        Config config;
        fs::path configPath = rootPath / "scanner.conf";

        if (!config.load(configPath.string())) {
            std::cerr << "[Scanner] Could not load config, using defaults.\n";
        } else {
            cout << "[Scanner] Loaded configuration from " << configPath << "\n";
        }

        double warningThreshold = config.getDouble("warning_threshold", 20.0);
        double criticalThreshold = config.getDouble("critical_threshold", 30.0);
        std::string mockDataDir = config.get("mock_data_dir", "tests/mock_data");
        std::string logsDirStr = config.get("logs_dir", "logs");
        bool showNormal = config.getBool("show_normal", false);

        fs::path mockDataPath = rootPath / mockDataDir;
        fs::path logsDir = rootPath / logsDirStr;
        fs::create_directories(logsDir);

        // ------------------------------------------------------------
        // Build log file path
        // ------------------------------------------------------------
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

        fs::path logFilePath = logsDir / logName.str();
        Logger logger(logFilePath.string());

        cout << "Scanning Directory: " << mockDataPath << "\n\n";
        logger.write("=== New Scan Started ===");
        logger.write("Loaded Config: " + configPath.string());
        logger.write("Scanning Directory: " + mockDataPath.string());
        logger.writeDivider();

        // ------------------------------------------------------------
        // Run scanner
        // ------------------------------------------------------------
        std::vector<FileRecord> files = listFiles(mockDataPath, warningThreshold, criticalThreshold);

        cout << left << setw(26) << "File"
             << setw(12) << "Size(GB)"
             << "Category\n";
        cout << string(60, '-') << '\n';

        int flagged = 0;
        for (const auto& f : files) {
            bool isNormal = f.category.empty();

            if (isNormal) {
                logger.write(f.name + " | " + f.path + " | " + to_string(f.sizeGB) + " GB | NORMAL");
                if (!showNormal) continue; // skip console print if disabled
            }

            cout << left << setw(26) << f.name
                 << setw(12) << fixed << setprecision(2) << f.sizeGB
                 << (isNormal ? "NORMAL" : f.category) << '\n';

            std::ostringstream oss;
            oss << f.name << " | " << f.path << " | "
                << std::fixed << std::setprecision(2) << f.sizeGB << " GB | "
                << (isNormal ? "NORMAL" : f.category);
            logger.write(oss.str());

            if (!isNormal) flagged++;
        }

        // ------------------------------------------------------------
        // Summary
        // ------------------------------------------------------------
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
