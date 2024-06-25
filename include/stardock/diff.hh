#ifndef DIFF_HH
#define DIFF_HH
/** @file diff.hh */
#include <stardock/point.hh>
#include <vector>
#include <string>
namespace stardock {
  struct Diff {
    std::vector<std::string> transfer_to_dst;
    std::vector<std::string> transfer_to_src;

    void asymmetric(const stardock::Point& src, const stardock::Point& dst);
    void symmetric(const stardock::Point& src, const stardock::Point& dst);
  };
}

std::ostream& operator<<(std::ostream& out, const stardock::Diff& diff);
#endif//DIFF_HH
