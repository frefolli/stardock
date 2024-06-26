#include <stardock/diff.hh>

void stardock::Diff::asymmetric(const stardock::Point& src, const stardock::Point& dst) {
  transfer_to_src.clear();
  transfer_to_dst.clear();

  uint32_t src_length = src.index.entries.size();
  uint32_t dst_length = dst.index.entries.size();
  uint32_t i = 0, j = 0;

  while(i < src_length && j < dst_length) {
    const stardock::Index::Entry& src_entry = src.index.entries[i];
    const stardock::Index::Entry& dst_entry = dst.index.entries[j];
    if (src_entry.path == dst_entry.path) {
      if (src_entry.modified > dst_entry.modified) {
        transfer_to_dst.push_back(src_entry.path);
      }
      ++i; ++j;
    } else if (src_entry.path > dst_entry.path) {
      ++j;
    } else {// src_entry.path < dst_entry.path
      transfer_to_dst.push_back(src_entry.path);
      ++i;
    }
  }

  while(i < src_length) {
    const stardock::Index::Entry& src_entry = src.index.entries[i];
    transfer_to_dst.push_back(src_entry.path);
    ++i;
  }
}

void stardock::Diff::symmetric(const stardock::Point& src, const stardock::Point& dst) {
  transfer_to_src.clear();
  transfer_to_dst.clear();

  uint32_t src_length = src.index.entries.size();
  uint32_t dst_length = dst.index.entries.size();
  uint32_t i = 0, j = 0;

  while(i < src_length && j < dst_length) {
    const stardock::Index::Entry& src_entry = src.index.entries[i];
    const stardock::Index::Entry& dst_entry = dst.index.entries[j];
    if (src_entry.path == dst_entry.path) {
      if (src_entry.modified > dst_entry.modified) {
        transfer_to_dst.push_back(src_entry.path);
      } else if (src_entry.modified < dst_entry.modified) {
        transfer_to_src.push_back(dst_entry.path);
      }
      ++i; ++j;
    } else if (src_entry.path > dst_entry.path) {
      transfer_to_src.push_back(dst_entry.path);
      ++j;
    } else {// src_entry.path < dst_entry.path
      transfer_to_dst.push_back(src_entry.path);
      ++i;
    }
  }

  while(i < src_length) {
    const stardock::Index::Entry& src_entry = src.index.entries[i];
    transfer_to_dst.push_back(src_entry.path);
    ++i;
  }

  while(j < dst_length) {
    const stardock::Index::Entry& dst_entry = dst.index.entries[j];
    transfer_to_src.push_back(dst_entry.path);
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
