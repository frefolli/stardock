#ifndef INDEX_HH
#define INDEX_HH
/** @file index.hh */
#include <cstdint>
#include <ostream>
#include <string>

namespace stardock {
  struct Index {
    struct Entry {
      uint64_t path_index;
      uint64_t file_size;
      uint64_t modified;
    };
    char* paths = nullptr;
    Entry* entries = nullptr;
    uint64_t n_of_entries = 0;
    uint64_t n_of_chars = 0;
    uint64_t entries_size = 0;
    uint64_t paths_size = 0;

    void push_back(std::string path, uint64_t file_size, uint64_t modified);

    inline const char* path(uint64_t entry_index) const {
      return paths + entries[entry_index].path_index;
    }
    bool dump(std::string path) const;
    bool load(std::string path);
    void clear();
    void sort();
  };
}

std::ostream& operator<<(std::ostream& out, const stardock::Index& index);
#endif//INDEX_HH
