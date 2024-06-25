#include <stardock/transfer.hh>
#include <iostream>
#include <filesystem>

inline void SendFiles(const stardock::Point& src, const stardock::Point& dst, const std::vector<std::string>& paths, bool verbose) {
  std::string old_dir = "";
  for (const std::string& path : paths) {
    std::string parent_path = std::filesystem::path(path).parent_path();
    if (parent_path != old_dir) {
      std::string concrete_path = dst.location + "/" + parent_path;
      if (!std::filesystem::exists(concrete_path)) {
        if (verbose) {
          std::cout << "creating " << concrete_path << std::endl;
        }
        std::filesystem::create_directories(concrete_path);
      }
      old_dir = parent_path;
    }
    
    if (verbose) {
      std::cout << "sending " << path << " to " << dst.location << std::endl;
    }
    std::string src_path = src.location + "/" + path;
    std::string dst_path = dst.location + "/" + path;
    std::filesystem::copy_file(src_path, dst_path);
  }
}

void stardock::Transfer::apply(bool verbose) {
  SendFiles(src, dst, diff.transfer_to_dst, verbose);
  SendFiles(dst, src, diff.transfer_to_src, verbose);
}
