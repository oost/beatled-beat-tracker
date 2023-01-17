#ifndef BTRACK__TRANSFORMERS__MAP_HPP
#define BTRACK__TRANSFORMERS__MAP_HPP

#include <complex>
#include <span>

#include "transformers/transformers/all.h"

namespace transformers {

template <typename I, typename O>
class MapTransformer : public BufferedTransformer<I, O> {
public:
  MapTransformer(std::size_t frameSize, std::function<O(I)> fn)
      : BufferedTransformer<I, O>(frameSize), fn_{fn} {}

protected:
  void process() override {
    if (this->input_buffer_->size() != this->output_buffer_->size()) {
      throw std::range_error("Frame size should be less of same size");
    }

    // copy into complex array and zero pad
    for (int i = 0; i < this->input_buffer_->size(); i++) {
      (*this->output_buffer_)[i] = fn_((*this->input_buffer_)[i]);
    }
  }

  std::function<O(I)> fn_;
};
} // namespace transformers

#endif // BTRACK__TRANSFORMERS__MAP_HPP