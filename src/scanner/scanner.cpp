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
#include <vector>

using namespace std;

namespace fs = filesystem;


// Hold file metadata for each OST file discovered
struct FileRecord
{
    string name;
    string path;
    double fileSize{0.0};
    string unit;

    FileRecord() : name(""), path(""), fileSize(0.0), unit("") {}
};


vector<FileRecord> listFiles(const fs::path& dirPath)
{

    vector<FileRecord> fileList;

    try
    {
        // Create a recursive iterator to walk through all subdirectories.
        // The flag `skip_permission_denied` ensures that protected folders won’t interrupt execution.
        for (const auto& entry : fs::recursive_directory_iterator(dirPath, fs::directory_options::skip_permission_denied))
        {
            // Extract the file extension for filtering (.ost or .OST)
            auto ext = entry.path().extension().string();

            // Filter condition: case insensitive OST files
            if (entry.is_regular_file() && (ext == ".ost" || ext == ".OST"))
            {
                FileRecord record;
                record.name = entry.path().filename().string();
                record.path = entry.path().string();

                // Get file size in bytes
                double size = static_cast<double>(fs::file_size(entry.path()));
                record.unit = "B";

                // Convert bytes to KB / MB / GB dynamically
                if (size >= 1024) {
                    size /= 1024;
                    record.unit = "KB";
                }
                if (size >= 1024) {
                    size /= 1024;
                    record.unit = "MB";
                }
                if (size >= 1024) {
                    size /= 1024;
                    record.unit = "GB";
                }

                record.fileSize = size;

                fileList.push_back(record);
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        // Catch and report filesystem errors, such as permission denial or I/O issues.
        cerr << "[Error] " << e.what() << '\n';
    }
    return fileList;
}

int main()
{
  // Define the base directory to scan.
  const fs::path mockDataPath = "../../tests/mock_data";

  cout << "=== OST Scanner Prototype v0.2.0 ===\n";
  cout << "Scanning Directory: " << mockDataPath.string() << "\n\n";

  vector<FileRecord> results = listFiles(mockDataPath);
  for (const auto& file : results)
  {
    cout << "File: " << file.name << '\n'
         << "Path: " << file.path << '\n'
         << "Size: " << fixed << setprecision(2)
         << file.fileSize << ' ' << file.unit << "\n\n";
  }

  cout << "=== Scan Complete ===\n";
  cout << "Total Files Found: " << results.size() << '\n';
}


