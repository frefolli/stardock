#ifndef MEMORY_HH
#define MEMORY_HH
/** @file memory.hh */
namespace stardock {
  #define IF_DIRTY_FREE_PTR(ptr) \
    if (ptr != nullptr) { \
      free(ptr); \
      ptr = nullptr;\
    }
}
#endif//MEMORY_HH
