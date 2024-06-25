#ifndef TRANSFER_HH
#define TRANSFER_HH
/** @file transfer.hh */
#include <stardock/diff.hh>
namespace stardock {
  struct Transfer {
    const stardock::Point& src;
    const stardock::Point& dst;
    const stardock::Diff& diff;

    void apply(bool verbose = false);
  };
}
#endif//TRANSFER_HH
