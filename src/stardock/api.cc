#include <cstdlib>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stardock.hh>

void Open(stardock::Point& point, std::string location, bool refresh = false) {
  if (refresh || !point.load(location)) {
    point.create(location);
  }
}

inline void DoIndex(const stardock::Config& config, std::string path) {
  stardock::Point point;
  Open(point, path, config.refresh);
  if (config.verbose) {
    std::cout << "Indexed " << point.location << std::endl;
    std::cout << "Indexed " << point.index.n_of_entries << " files" << std::endl;
    std::cout << point.index << std::endl;
  }
}

template<typename T>
void Flush(const T& diff, std::string filepath) {
  std::ofstream out (filepath);
  out << diff << std::endl;
  out.close();
}

inline void DoTransfer(const stardock::Config& config, const stardock::Point& src_point,
                       const stardock::Point& dst_point, const stardock::Diff& diff) {
  stardock::Transfer transfer {
    .src = src_point,
    .dst = dst_point,
    .diff = diff
  };
  transfer.apply(config.verbose);
}

inline void DoAlign(const stardock::Config& config, const stardock::Point& src_point,
                       const stardock::Point& dst_point, const stardock::Diff& diff) {
  stardock::Align align {
    .src = src_point,
    .dst = dst_point,
    .diff = diff
  };
  align.apply(config.verbose);
}

inline void DoDiff(const stardock::Config& config) {
  assert(!(config.asym_diff && config.sym_diff));
  assert(config.src.size() > 0);
  assert(config.dst.size() > 0);
  
  stardock::Point src_point, dst_point;
  Open(src_point, config.src, config.refresh);
  Open(dst_point, config.dst, config.refresh);

  stardock::Diff diff;
  if (config.asym_diff) {
    diff.asymmetric(src_point, dst_point);
  } else {
    diff.symmetric(src_point, dst_point);
  }

  if (config.transfer) {
    DoTransfer(config, src_point, dst_point, diff);
    // TODO: merge indexes instead
    DoIndex(config, src_point.location);
    DoIndex(config, dst_point.location);
  } else if (config.align) {
    DoAlign(config, src_point, dst_point, diff);
    // TODO: merge indexes instead
    DoIndex(config, src_point.location);
    DoIndex(config, dst_point.location);
  } else {
    if (config.verbose) {
      std::cout << "src: " << config.src << std::endl;
      std::cout << "dst: " << config.dst << std::endl;
      std::cout << diff << std::endl;
    }
  }
}

void stardock::Exec(const stardock::Config& config) {
  if (config.asym_diff || config.sym_diff) {
    DoDiff(config);
  } else if (config.index) {
    assert(config.src.size() > 0 || config.dst.size() > 0);
    if (config.src.size() > 0) {
      DoIndex(config, config.src);
    }
    if (config.dst.size() > 0) {
      DoIndex(config, config.dst);
    }
  } else if (config.transfer) {
    std::cerr << "transfer action needs one of the Sym/Asym options supplied" << std::endl;
    std::exit(1);
  }
}
