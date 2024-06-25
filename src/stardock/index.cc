#include <stardock/index.hh>
#include <fstream>
#include <iostream>

void stardock::Index::Entry::dump(std::ofstream& out) const {
  out.write(reinterpret_cast<const char*>(&file_size), sizeof(file_size));
  out.write(reinterpret_cast<const char*>(&modified), sizeof(modified));
  uint64_t length = path.size();
  out.write(reinterpret_cast<const char*>(&length), sizeof(length));
  out.write(path.c_str(), length);
}

void stardock::Index::Entry::load(std::ifstream& in) {
  in.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));
  in.read(reinterpret_cast<char*>(&modified), sizeof(modified));
  uint64_t length = 0;
  in.read(reinterpret_cast<char*>(&length), sizeof(length));
  path.resize(length);
  in.read(path.data(), length);
}

bool stardock::Index::dump(std::string path) const {
  std::ofstream out(path, std::ios::binary | std::ios::out | std::ios::trunc);
  if (out.is_open()) {
    const uint64_t length = entries.size();
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    for (uint64_t i = 0; i < length; ++i) {
      entries[i].dump(out);
    }
    out.close();
    return true;
  }
  return false;
}

bool stardock::Index::load(std::string path) {
  entries.clear();
  std::ifstream in(path, std::ios::binary | std::ios::in);
  if (in.is_open()) {
    uint64_t length = 0;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    entries.resize(length);
    for (uint64_t i = 0; i < length; ++i) {
      entries[i].load(in);
    }
    in.close();
    return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& out, const stardock::Index& index) {
  bool first = true;
  for (const stardock::Index::Entry& entry : index.entries) {
    if (first) {
      first = false;
    } else {
      out << std::endl;
    }
    out << entry;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const stardock::Index::Entry& entry) {
  return out << entry.path << " | " << entry.file_size << " | " << entry.modified;
}
