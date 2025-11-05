#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
// fs::path path("../../tests/mock_data");

void listFiles(const fs::path& dirPath) {
  for (const auto& entry: fs::recursive_directory_iterator(dirPath, fs::directory_options::skip_permission_denied)){
        
    std::cout << "Name: " << entry.path().filename() << '\n'
              << "Path: " << entry.path() << '\n';

  if (fs::is_regular_file(entry.path())){
    try {
    std::cout << "Size: " << fs::file_size(entry.path()) << " gb\n\n";
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

int main()
{
  // for (auto const& dir_entry : fs::recursive_directory_iterator(path))
  //   std::cout << dir_entry << '\n';
  listFiles("../../tests/mock_data");
}


