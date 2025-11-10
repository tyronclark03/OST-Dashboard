#include "config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// --- Load configuration file ---
bool Config::load(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[Config] Failed to open config file: " << filePath << '\n';
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty or comment lines
        if (line.empty() || line[0] == '#') continue;

        // Parse key=value
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        // Trim again after split
        key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);

        values[key] = value;
    }

    file.close();
    return true;
}

// --- Retrieve as string ---
std::string Config::get(const std::string& key, const std::string& defaultValue) const {
    auto it = values.find(key);
    return (it != values.end()) ? it->second : defaultValue;
}


// --- Retrieve as integer ---
int Config::getInt(const std::string& key, int defaultValue) const {
    try {
        return std::stoi(get(key, std::to_string(defaultValue)));
    } catch (...) {
        return defaultValue;
    }
}


// --- Retrieve as double ---
double Config::getDouble(const std::string& key, double defaultValue) const {
    try {
        return std::stod(get(key, std::to_string(defaultValue)));
    } catch (...) {
        return defaultValue;
    }
}

// --- Retrieve as boolean ---
bool Config::getBool(const std::string& key, bool defaultValue) const {
    std::string val = get(key, defaultValue ? "true" : "false");
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return (val == "true" || val == "1" || val == "yes");
}

// --- Print summary ---
void Config::printSummary() const {
    std::cout << "\n=== Loaded Configuration ===\n";
    for (const auto& [key, value] : values) {
        std::cout << key << " = " << value << '\n';
    }
    std::cout << "=============================\n";
}
