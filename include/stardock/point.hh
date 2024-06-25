#ifndef POINT_HH
#define POINT_HH
/** @file point.hh */
#include <stardock/index.hh>

namespace stardock {
  struct Point {
    std::string location;
    Index index;

    bool load(std::string location);
    bool create(std::string location);

    private:
    void build(std::string subdir);
  };
}
#endif//POINT_HH
