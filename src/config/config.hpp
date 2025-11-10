#pragma once
#include <string>
#include <unordered_map>

class Config {
private:
    std::unordered_map<std::string, std::string> values; // stores key=value pairs

public:
    // Load a configuration file into memory
    bool load(const std::string& filePath);

    // Retrieve a value as a string
    std::string get(const std::string& key, const std::string& defaultValue = "") const;

    // Retrieve as numeric types (with fallback defaults)
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;

    // Print all loaded config values for
    void printSummary() const;
};
