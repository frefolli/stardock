#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <stardock.hh>

inline bool AddBoolFlag(char** args, uint64_t /*argc*/, uint64_t& i, const char* short_flag, const char* long_flag, bool& recipient) {
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
    std::cout << "    -R/--align        do align (according to diff)" << std::endl;
    std::cout << "    -I/--index        do index (preventive for transfer and diff)" << std::endl;
    std::exit(0);
}

int main(int argc, char** args) {
  stardock::Config cli_config;
  for (uint64_t i = 1; i < (uint64_t) argc; ++i) {
    if (!(AddBoolFlag(args, argc, i, "-h", "--help", cli_config.help)
       || AddBoolFlag(args, argc, i, "-r", "--refresh", cli_config.refresh)
       || AddBoolFlag(args, argc, i, "-v", "--verbose", cli_config.verbose)
       || AddStringFlag(args, argc, i, "-s", "--src", cli_config.src)
       || AddStringFlag(args, argc, i, "-d", "--dst", cli_config.dst)
       || AddBoolFlag(args, argc, i, "-A", "--asym", cli_config.asym_diff)
       || AddBoolFlag(args, argc, i, "-S", "--sym", cli_config.sym_diff)
       || AddBoolFlag(args, argc, i, "-T", "--transfer", cli_config.transfer)
       || AddBoolFlag(args, argc, i, "-R", "--align", cli_config.transfer)
       || AddBoolFlag(args, argc, i, "-I", "--index", cli_config.index)
    )) {
      std::cerr << "unknown argument '" << args[i] << "'" << std::endl;
      std::exit(1);
    }
  }
  if (cli_config.help) {
    PrintHelp(args[0]);
  }
  stardock::Exec(cli_config);
}
