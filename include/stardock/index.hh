#ifndef INDEX_HH
#define INDEX_HH
/** @file index.hh */
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace stardock {
  struct Index {
    struct Entry {
      std::string path;
      uint64_t file_size;
      uint64_t modified;

      void dump(std::ofstream& out) const;
      void load(std::ifstream& in);

      inline bool operator<(const Entry& other) {
        return path < other.path;
      }
    };
    std::vector<Entry> entries;

    bool dump(std::string path) const;
    bool load(std::string path);
  };
}

std::ostream& operator<<(std::ostream& out, const stardock::Index& index);
std::ostream& operator<<(std::ostream& out, const stardock::Index::Entry& index);
#endif//INDEX_HH
