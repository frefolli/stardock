#ifndef ALIGN_HH
#define ALIGN_HH
/** @file align.hh */
#include <stardock/diff.hh>
namespace stardock {
  struct Align {
    const stardock::Point& src;
    const stardock::Point& dst;
    const stardock::Diff& diff;

    void apply(bool verbose = false);
  };
}
#endif//ALIGN_HH
