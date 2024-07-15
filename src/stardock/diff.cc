#include <stardock/diff.hh>
#include <cstring>

void stardock::Diff::asymmetric(const stardock::Point& src, const stardock::Point& dst) {
  transfer_to_src.clear();
  transfer_to_dst.clear();

  uint32_t src_length = src.index.n_of_entries;
  uint32_t dst_length = dst.index.n_of_entries;
  uint32_t i = 0, j = 0;

  while(i < src_length && j < dst_length) {
    const stardock::Index::Entry& src_entry = src.index.entries[i];
    const stardock::Index::Entry& dst_entry = dst.index.entries[j];
    const char* src_path = src.index.path(i);
    const char* dst_path = dst.index.path(j);
    if (std::strcmp(src_path, dst_path) == 0) {
      if (src_entry.modified > dst_entry.modified) {
        transfer_to_dst.push_back(src_path);
      }
      ++i; ++j;
    } else if (std::strcmp(src_path, dst_path) > 0) {
      ++j;
    } else {// src_path < dst_path
      transfer_to_dst.push_back(src_path);
      ++i;
    }
  }

  while(i < src_length) {
    const char* src_path = src.index.path(i);
    transfer_to_dst.push_back(src_path);
    ++i;
  }
}

void stardock::Diff::symmetric(const stardock::Point& src, const stardock::Point& dst) {
  transfer_to_src.clear();
  transfer_to_dst.clear();

  uint32_t src_length = src.index.n_of_entries;
  uint32_t dst_length = dst.index.n_of_entries;
  uint32_t i = 0, j = 0;

  while(i < src_length && j < dst_length) {
    const stardock::Index::Entry& src_entry = src.index.entries[i];
    const stardock::Index::Entry& dst_entry = dst.index.entries[j];
    const char* src_path = src.index.path(i);
    const char* dst_path = dst.index.path(j);
    if (std::strcmp(src_path, dst_path) == 0) {
      if (src_entry.modified > dst_entry.modified) {
        transfer_to_dst.push_back(src_path);
      } else if (src_entry.modified < dst_entry.modified) {
        transfer_to_src.push_back(dst_path);
      }
      ++i; ++j;
    } else if (std::strcmp(src_path, dst_path) > 0) {
      transfer_to_src.push_back(dst_path);
      ++j;
    } else {// src_path < dst_path
      transfer_to_dst.push_back(src_path);
      ++i;
    }
  }

  while(i < src_length) {
    const char* src_path = src.index.path(i);
    transfer_to_dst.push_back(src_path);
    ++i;
  }

  while(j < dst_length) {
    const char* dst_path = dst.index.path(j);
    transfer_to_src.push_back(dst_path);
    ++j;
  }
}

std::ostream& operator<<(std::ostream& out, const stardock::Diff& diff) {
  for (std::string path : diff.transfer_to_dst) {
    out << "-> " << path << std::endl;
  }
  for (std::string path : diff.transfer_to_src) {
    out << "<- " << path << std::endl;
  }
  out << "total of " << diff.transfer_to_dst.size() + diff.transfer_to_src.size() << " transfers";
  return out;
}
