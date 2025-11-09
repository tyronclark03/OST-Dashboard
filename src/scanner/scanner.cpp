/*
 * Module: Scanner
 * Version: v0.3.0
 * Description:
 *  Traverses mock data directory, gathers metadata, and classifies files by size thresholds.
 * Last update: 2025-11-08
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
    string name{};
    string path{};
    double fileSize{0.0};
    string unit{};
    string category{};

    // FileRecord() : name(""), path(""), fileSize(0.0), unit(""), category("") {}
};

string classifyFileSize(double sizeGB)
{
    if(sizeGB > 30.0)
    {
        return "CRITICAL";
    }
    else if (sizeGB >= 10.0)
    {
        return "WARNING";
    }
    else
    {
        return "NORMAL";
    }
}

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

                // Convert bytes to KB / MB / GB
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
                record.category = classifyFileSize(size);

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

  cout << "=== OST Scanner ===\n";
  cout << "Scanning Directory: " << mockDataPath.string() << "\n\n";

  vector<FileRecord> results = listFiles(mockDataPath);

  cout << left << setw(25) << "File"
       << setw(12) << "Size(GB)"
       << "Category\n";
       
  cout << string(60, '-') << '\n';

  int flaggedCount = 0;
  for (const auto& file : results)
  {
    // Does not display normal sized files
    if (file.category == "NORMAL")
    {
        continue;
    }

    cout << left << setw(25) << file.name
         << setw(12) << fixed << setprecision(2) << file.fileSize
         << file.category << '\n';
    flaggedCount++;
   }
  cout << '\n';
  cout << "=== Scan Complete ===\n";
  cout << "Total Files Found: " << results.size() << '\n';
  cout << "Files Exceeding Limit: " << flaggedCount << '\n';
}


