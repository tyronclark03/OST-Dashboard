#include "action_handler.hpp"
#include <iostream>

ActionHandler::ActionHandler(const Config& configRef, Logger& loggerRef)
    : config(configRef), logger(loggerRef)
{
    // Load behavior options from configuration
    criticalAction = config.get("action_on_critical", "log_only");
    warningAction  = config.get("action_on_warning", "log_only");
    archiveDir     = config.get("archive_dir", "archive");
    dryRun         = config.getBool("dry_run", true);
}

/*
 * Core action dispatcher
 * - Evaluates file category
 * - Routes to appropriate operation
 */
void ActionHandler::handleFile(const FileRecord& record) {
    std::string actionType;

    if (record.category == "CRITICAL") actionType = criticalAction;
    else if (record.category == "WARNING") actionType = warningAction;
    else return; // no action for normal files

    logger.write("[Action] " + record.name + " → " + actionType);

    if (dryRun) {
        std::cout << "[Dry Run] Would perform: " << actionType << " on " << record.name << '\n';
        return;
    }

    if (actionType == "move") performMove(record);
    else if (actionType == "delete") performDelete(record);
    else performLogOnly(record);
}

/*
 * Perform file move
 * - Intended to relocate large OSTs to archive directory
 */
void ActionHandler::performMove(const FileRecord& record) {
    std::cout << "[Action] Moving: " << record.path << " → " << archiveDir << '\n';
    logger.write("[Move] " + record.path + " → " + archiveDir);
}

/*
 * Perform file deletion
 * - Should verify path safety before deletion in future versions
 */
void ActionHandler::performDelete(const FileRecord& record) {
    std::cout << "[Action] Deleting: " << record.path << '\n';
    logger.write("[Delete] " + record.path);
}

/*
 * Default: Log-only action (no file modification)
 */
void ActionHandler::performLogOnly(const FileRecord& record) {
    logger.write("[LogOnly] " + record.path + " | " + std::to_string(record.sizeGB) + " GB");
}
