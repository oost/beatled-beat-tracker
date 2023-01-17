#ifndef BTRACK__TRANSFORMER__BUFFERS__SINGLE_VALUE_BUFFER_HPP
#define BTRACK__TRANSFORMER__BUFFERS__SINGLE_VALUE_BUFFER_HPP

#include "buffer.h"

namespace transformers {

template <typename T> class SingleValueBuffer : public Buffer {
public:
  using Ptr = std::shared_ptr<SingleValueBuffer>;
  using CPtr = std::shared_ptr<const SingleValueBuffer>;
  using value_t = T;

  SingleValueBuffer() = default;
  SingleValueBuffer(const T &v) : value_{v} {}
  // T &value() { return value_; }
  const T &value() const { return value_; }
  void set_value(const T &v) { value_ = v; }

private:
  T value_;
};

} // namespace transformers

#endif // BTRACK__TRANSFORMER__BUFFERS__SINGLE_VALUE_BUFFER_HPP