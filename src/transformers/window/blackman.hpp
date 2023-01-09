#ifndef BTRACK__WINDOWS__BLACKMAN_WINGOW_HPP
#define BTRACK__WINDOWS__BLACKMAN_WINGOW_HPP

#include "transformers/window_function.hpp"

namespace transformers {

class BlackmanWindow : public WindowTransformer {
public:
  BlackmanWindow(std::size_t windowSize) : WindowTransformer(windowSize) {
    double N = static_cast<double>(windowSize - 1);
    // variable to store frameSize_ minus 1

    // Blackman window calculation
    for (int n = 0; n < windowSize; n++) {
      window_[n] =
          0.42 -
          (0.5 * cos(2 * std::numbers::pi * (static_cast<double>(n) / N))) +
          (0.08 * cos(4 * std::numbers::pi * (static_cast<double>(n) / N)));
    }
  }
};

} // namespace transformers

#endif // BTRACK__WINDOWS__BLACKMAN_WINGOW_HPP