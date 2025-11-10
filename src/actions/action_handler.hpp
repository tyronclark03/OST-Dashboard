#pragma once
#include <string>
#include <filesystem>
#include "../scanner/scanner.hpp"
#include "../config/config.hpp"
#include "../logger/logger.hpp"

namespace fs = std::filesystem;

class ActionHandler {
public:
    // Initialize handler with references to Config + Logger
    ActionHandler(const Config& config, Logger& logger);

    // Perform configured action on a given file
    void handleFile(const FileRecord& record);

private:
    const Config& config;
    Logger& logger;

    // Resolved configuration values
    std::string criticalAction;  // "move" or "delete"
    std::string warningAction;   // "log_only"
    std::string archiveDir;      // optional target for moved files
    bool dryRun;                 // whether to simulate actions

    // Internal helpers
    void performMove(const FileRecord& record);
    void performDelete(const FileRecord& record);
    void performLogOnly(const FileRecord& record);
};
