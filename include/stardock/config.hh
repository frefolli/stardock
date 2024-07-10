#ifndef CONFIG_HH
#define CONFIG_HH
/** @file config.hh */
#include <string>
namespace stardock {
  struct Config {
    bool help = false;
    bool refresh = false;
    bool verbose = false;
    bool asym_diff = false;
    bool sym_diff = false;
    bool transfer = false;
    bool align = false;
    bool index = false;
    std::string src = "";
    std::string dst = "";
  };
}
#endif//CONFIG_HH
