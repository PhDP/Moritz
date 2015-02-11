#ifndef WAGNER2_UTILS_HH_
#define WAGNER2_UTILS_HH_

#include <iostream>
#include <sstream>
#include <cinttypes>

namespace wagner {

/**
* \brief Converts any type to a string of bits (with spaces between every byte).
*/
template <typename T>
std::string showBits(const T &t) {
  std::stringstream ss;
  for (size_t i = 0; i < sizeof(T); ++i) {
    const uint8_t byte = *((uint8_t *)&t + i);
    for (uint8_t bit = 0x80; bit > 0x00; bit >>= 1)
      ss << ((byte & bit) ? '1' : '0');
    ss << " ";
  }
  return ss.str();
}

}

#endif /* WAGNER2_UTILS_HH_ */
