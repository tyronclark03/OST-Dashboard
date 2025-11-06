/*
 * Module: Scanner
 * Version: v0.2.0
 * Description:
 *   Performs recursive traversal of OST directories using the C++17 filesystem library.
 *   Filters for .ost and .OST files only, then displays file path, name, and formatted size.
 *   Handles permission errors gracefully.
 * Last update: 2025-11-05
 */

#include <iostream>
#include <filesystem>
#include <iomanip>
#include <string>

namespace fs = std::filesystem;


void listFiles(const fs::path& dirPath)
{
    try
    {
        // Create a recursive iterator to walk through all subdirectories.
        // The flag `skip_permission_denied` ensures that protected folders won’t interrupt execution.
        for (const auto& entry : fs::recursive_directory_iterator(dirPath, fs::directory_options::skip_permission_denied))
        {
            // Extract the file extension for filtering (.ost or .OST)
            auto ext = entry.path().extension().string();

            // Filter condition: case insensitive OST files
            if (ext == ".ost" || ext == ".OST")
            {
                // Confirm the entry is a regular file (not directory, symlink, etc.)
                if (fs::is_regular_file(entry.path()))
                {
                    // Get file size in bytes
                    auto size = fs::file_size(entry.path());
                    double readableSize = static_cast<double>(size);
                    std::string unit = "B";

                    // Convert bytes to KB / MB / GB dynamically
                    if (readableSize >= 1024) {
                        readableSize /= 1024;
                        unit = "KB";
                    }
                    if (readableSize >= 1024) {
                        readableSize /= 1024;
                        unit = "MB";
                    }
                    if (readableSize >= 1024) {
                        readableSize /= 1024;
                        unit = "GB";
                    }

                    // Output formatted file details
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << "File: " << entry.path().filename().string() << '\n';
                    std::cout << "Path: " << entry.path().string() << '\n';
                    std::cout << "Size: " << readableSize << ' ' << unit << "\n\n";
                }
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        // Catch and report filesystem errors, such as permission denial or I/O issues.
        std::cerr << "[Error] " << e.what() << '\n';
    }
}

int main()
{
  // Define the base directory to scan.
  const fs::path mockDataPath = "../../tests/mock_data";

  std::cout << "=== OST Scanner Prototype v0.2.0 ===\n";
  std::cout << "Scanning Directory: " << mockDataPath.string() << "\n\n";

  // Perform recursive directory traversal.
  listFiles(mockDataPath);

  std::cout << "=== Scan Complete ===\n";
}


