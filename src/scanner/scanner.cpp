#include <iostream>
#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem;
// fs::path path("../../tests/mock_data");

void listFiles(const fs::path& dirPath) {
  for (const auto& entry: fs::recursive_directory_iterator(dirPath, fs::directory_options::skip_permission_denied)){
    if (entry.path().extension() == ".ost"){      
        std::cout << "Name: " << entry.path().filename() << '\n'
                  << "Path: " << entry.path() << '\n';

      if (fs::is_regular_file(entry.path())){
        try {
        uintmax_t size = fs::file_size(entry.path());
        double displaySize = static_cast<double>(size);
        std::string unit;

        if (size < 1024) {
            unit = "bytes";
        } 
        else if (size < 1024 * 1024) {
            displaySize /= 1024.0;
            unit = "KB";
        } 
        else if (size < 1024ULL * 1024 * 1024) {
            displaySize /= (1024.0 * 1024);
            unit = "MB";
        } 
        else {
            displaySize /= (1024.0 * 1024 * 1024);
            unit = "GB";
        }       
        
        std::cout << "Size: " << std::fixed << std::setprecision(2) << displaySize << " " << unit << "\n\n";
        // std::cout << "Size: " << fs::file_size(entry.path()) << " bytes\n\n";
        } catch (const fs::filesystem_error& e){
        std::cerr << "Error reading size: " << e.what() << '\n';
        }
      } 

      else if (fs::is_directory(entry.path())){
        std::cout << "Type: Directory\n\n";
      }

      else{
        std::cout <<"Type: Other (symlink, socket, etc.)\n\n";
      }
    }                
  }
}

int main()
{
  // for (auto const& dir_entry : fs::recursive_directory_iterator(path))
  //   std::cout << dir_entry << '\n';
  listFiles("../../tests/mock_data");
}


