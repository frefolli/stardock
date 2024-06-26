#include <algorithm>
#include <stardock/point.hh>
#include <sys/stat.h>
#include <string>
#include <filesystem>

#define IGNORE_HIDDEN_FILES
#define IGNORE_SYMLINKS

void stardock::Point::build(std::string subdir) {
  for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(subdir)) {
    const std::filesystem::path& path = entry.path();
    #ifdef IGNORE_HIDDEN_FILES
      if (path.filename().string()[0] == '.') {
        continue;
      }
    #endif
    #ifdef IGNORE_SYMLINKS
      if (entry.is_symlink()) {
        continue;
      }
    #endif
    if (entry.is_regular_file()) {
      struct stat filestat;
      stat(path.string().c_str(), &filestat);
      index.entries.push_back({
        .path = std::filesystem::relative(path, location),
        .file_size = (uint64_t)entry.file_size(),
        .modified = (uint64_t)filestat.st_mtim.tv_sec
      });
    } else if (entry.is_directory()) {
      build(path.string());
    }
  }
}

bool stardock::Point::load(std::string location) {
  this->location = location;
  std::string index_file = location + "/.index.bin";
  return index.load(index_file);
}

bool stardock::Point::create(std::string location) {
  this->location = location;
  std::string index_file = location + "/.index.bin";
  index.entries.clear();
  build(location);
  std::sort(index.entries.begin(), index.entries.end());
  return index.dump(index_file);
}
