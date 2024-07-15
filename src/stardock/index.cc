#include <cstdlib>
#include <cstring>
#include <stardock/index.hh>
#include <stardock/memory.hh>
#include <fstream>
#include <iostream>
#include <algorithm>

constexpr uint64_t INDEX_ENTRIES_INCREMENT = 50;
constexpr uint64_t INDEX_PATHS_INCREMENT = 10 * INDEX_ENTRIES_INCREMENT;

bool stardock::Index::dump(std::string path) const {
  std::ofstream out(path, std::ios::binary | std::ios::out | std::ios::trunc);
  if (out.is_open()) {
    out.write(reinterpret_cast<const char*>(&n_of_entries), sizeof(n_of_entries));
    out.write(reinterpret_cast<const char*>(&n_of_chars), sizeof(n_of_chars));
    out.write(reinterpret_cast<const char*>(entries), sizeof(Entry) * n_of_entries);
    out.write(reinterpret_cast<const char*>(paths), sizeof(char) * n_of_chars);
    out.close();
    return true;
  }
  return false;
}

bool stardock::Index::load(std::string path) {
  clear();
  std::ifstream in(path, std::ios::binary | std::ios::in);
  if (in.is_open()) {
    in.read(reinterpret_cast<char*>(&n_of_entries), sizeof(n_of_entries));
    in.read(reinterpret_cast<char*>(&n_of_chars), sizeof(n_of_chars));
    entries = (Entry*) std::malloc (sizeof(Entry) * n_of_entries);
    paths = (char*) std::malloc (sizeof(char) * n_of_chars);
    in.read(reinterpret_cast<char*>(entries), sizeof(Entry) * n_of_entries);
    in.read(reinterpret_cast<char*>(paths), sizeof(char) * n_of_chars);
    entries_size = n_of_entries;
    paths_size = n_of_chars;
    in.close();
    return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& out, const stardock::Index& index) {
  bool first = true;
  for (uint64_t i = 0; i < index.n_of_entries; ++i) {
    const stardock::Index::Entry& entry = index.entries[i];
    if (first) {
      first = false;
    } else {
      out << std::endl;
    }
    out << index.path(i) << " | " << entry.file_size << " | " << entry.modified;
  }
  return out;
}

void stardock::Index::push_back(std::string path, uint64_t file_size, uint64_t modified) {
  if (n_of_entries + 1 > entries_size) {
    entries_size += INDEX_ENTRIES_INCREMENT;
    entries = (Entry*) std::realloc(entries, sizeof(Entry) * entries_size);
  }
  
  uint64_t requested_path_size = n_of_chars + path.size() + 1;
  if (requested_path_size > paths_size) {
    paths_size += INDEX_PATHS_INCREMENT;
    while(requested_path_size > paths_size) {
      paths_size += INDEX_PATHS_INCREMENT;
    }
    paths = (char*) std::realloc(paths, sizeof(char) * paths_size);
  }

  Entry& entry = entries[n_of_entries];
  entry.modified = modified;
  entry.file_size = file_size;
  entry.path_index = n_of_chars;
  std::strcpy(paths + entry.path_index, path.c_str());
  (paths + entry.path_index)[path.size()] = '\0';

  n_of_entries += 1;
  n_of_chars += path.size() + 1;
}

void stardock::Index::clear() {
  IF_DIRTY_FREE_PTR(entries);
  IF_DIRTY_FREE_PTR(paths);
  n_of_chars = 0;
  n_of_entries = 0;
  entries_size = 0;
  paths_size = 0;
}

void stardock::Index::sort() {
  std::sort(entries, entries + n_of_entries, [this](const Entry& a, const Entry& b) {
    return strcmp(paths + a.path_index, paths + b.path_index) < 0;
  });
}
