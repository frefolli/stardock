#include "stardock/transfer.hh"
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stardock.hh>

struct CliConfig {
  bool help = false;
  bool refresh = false;
  bool verbose = false;
  bool asym_diff = false;
  bool sym_diff = false;
  bool transfer = false;
  std::string src = "";
  std::string dst = "";
};

void Open(stardock::Point& point, std::string location, bool refresh = false) {
  if (refresh || !point.load(location)) {
    point.create(location);
  }
}

template<typename T>
void Flush(const T& diff, std::string filepath) {
  std::ofstream out (filepath);
  out << diff << std::endl;
  out.close();
}

inline bool AddBoolFlag(char** args, uint64_t argc, uint64_t& i, const char* short_flag, const char* long_flag, bool& recipient) {
  if (std::strcmp(short_flag, args[i]) == 0 || std::strcmp(long_flag, args[i]) == 0) {
    recipient = true;
    return true;
  }
  return false;
}

inline bool AddStringFlag(char** args, uint64_t argc, uint64_t& i, const char* short_flag, const char* long_flag, std::string& recipient) {
  if (std::strcmp(short_flag, args[i]) == 0 || std::strcmp(long_flag, args[i]) == 0) {
    ++i;
    if (i >= argc) {
      std::cerr << long_flag << " expects an argument" << std::endl;
      std::exit(1);
    }
    recipient = args[i];
    return true;
  }
  return false;
}

inline void PrintHelp(const char* exe) {
    std::cout << exe << " [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "    -h/--help         print help (this)" << std::endl;
    std::cout << "    -r/--refresh      always refreshes indexes" << std::endl;
    std::cout << "    -v/--verbose      uses verbose log" << std::endl;
    std::cout << "    -s/--src          specify src path" << std::endl;
    std::cout << "    -d/--dst          specify dst path" << std::endl;
    std::cout << "    -A/--asym         do asymmetric diff (src --> dst)" << std::endl;
    std::cout << "    -S/--sym          do symmetric diff (src <-> dst)" << std::endl;
    std::cout << "    -T/--transfer     do transfer (according to diff)" << std::endl;
    std::exit(0);
}

int main(int argc, char** args) {
  CliConfig cli_config;
  for (uint64_t i = 1; i < argc; ++i) {
    if (!(AddBoolFlag(args, argc, i, "-h", "--help", cli_config.help)
       || AddBoolFlag(args, argc, i, "-r", "--refresh", cli_config.refresh)
       || AddBoolFlag(args, argc, i, "-v", "--verbose", cli_config.verbose)
       || AddStringFlag(args, argc, i, "-s", "--src", cli_config.src)
       || AddStringFlag(args, argc, i, "-d", "--dst", cli_config.dst)
       || AddBoolFlag(args, argc, i, "-A", "--asym", cli_config.asym_diff)
       || AddBoolFlag(args, argc, i, "-S", "--sym", cli_config.sym_diff)
       || AddBoolFlag(args, argc, i, "-T", "--transfer", cli_config.transfer)
    )) {
      std::cerr << "unknown argument '" << args[i] << "'" << std::endl;
      std::exit(1);
    }
  }
  
  if (cli_config.help) {
    PrintHelp(args[0]);
  }

  assert(!(cli_config.asym_diff && cli_config.sym_diff));

  if (cli_config.asym_diff || cli_config.sym_diff) {
    assert(cli_config.src != "");
    assert(cli_config.dst != "");
    
    stardock::Point src_point, dst_point;
    Open(src_point, cli_config.src, cli_config.refresh);
    Open(dst_point, cli_config.dst, cli_config.refresh);

    stardock::Diff diff;
    if (cli_config.asym_diff) {
      diff.asymmetric(src_point, dst_point);
    } else {
      diff.symmetric(src_point, dst_point);
    }

    if (cli_config.transfer) {
      stardock::Transfer transfer {
        .src = src_point,
        .dst = dst_point,
        .diff = diff
      };
      transfer.apply(cli_config.verbose);
    } else {
      if (cli_config.verbose) {
        std::cout << "src: " << cli_config.src << std::endl;
        std::cout << "dst: " << cli_config.dst << std::endl;
        std::cout << diff << std::endl;
      }
    }
  } else {
    if (cli_config.transfer) {
      std::cerr << "transfer action needs one of the Sym/Asym options supplied" << std::endl;
      std::exit(1);
    }

    PrintHelp(args[0]);
  }
}
