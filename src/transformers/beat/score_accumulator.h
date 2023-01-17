#ifndef BTRACK__BEAT__SCORE_ACCUMULATOR_HPP
#define BTRACK__BEAT__SCORE_ACCUMULATOR_HPP

#include <memory>

#include "all.h"
#include "transformers/buffers/all.h"
#include "transformers/transformers/all.h"

namespace transformers {

/*!
 * \brief Score Accumulator
 *
 *
 * @param[in]  beat_period
 * @param[in]  onset_df_sample
 * @param[out] cumulative_score
 *
 */
class ScoreAccumulator
    : public GenericTransformer<MultiBuffer, CircularBuffer<double>> {
public:
  ScoreAccumulator(double tightness, double alpha, std::size_t buffer_length)
      : tightness_{tightness}, alpha_{alpha} {

    // Output
    this->output_buffer_ =
        std::make_shared<CircularBuffer<double>>(buffer_length);
  }

  virtual ~ScoreAccumulator() {}

protected:
  void input_updated() override {
    // Inputs
    beat_period_ptr = input_buffer_->buffer_cast<SingleValueBuffer<double>>(
        transformers::constants::beat_period_id);
    onset_df_sample_ptr = input_buffer_->buffer_cast<SingleValueBuffer<double>>(
        transformers::constants::odf_sample_id);
  }

  void process() override {
    double beat_period = beat_period_ptr->value();
    double onset_df_sample = onset_df_sample_ptr->value();

    int start, end, winsize;
    double max;

    start = output_buffer_->size() - round(2 * beat_period);
    end = start = output_buffer_->size() - round(beat_period / 2);
    winsize = end - start + 1;

    double w1[winsize];
    double v = -2 * beat_period;
    double wcumscore;

    // create window
    for (int i = 0; i < winsize; i++) {
      w1[i] = exp((-1 * pow(tightness_ * log(-v / beat_period), 2)) / 2);
      v = v + 1;
    }

    // calculate new cumulative score value
    max = 0;
    int n = 0;
    for (int i = start; i <= end; i++) {
      wcumscore = (*output_buffer_)[i] * w1[n];

      if (wcumscore > max) {
        max = wcumscore;
      }
      n++;
    }

    latest_cumulative_score_value_ =
        ((1 - alpha_) * onset_df_sample) + (alpha_ * max);

    output_buffer_->append(latest_cumulative_score_value_);
  }

  // Parameters
  double tightness_;
  double alpha_;

  // Internal state
  double latest_cumulative_score_value_ = 0;

  // Inputs
  SingleValueBuffer<double>::Ptr beat_period_ptr;
  SingleValueBuffer<double>::Ptr onset_df_sample_ptr;
};

} // namespace transformers

#endif // BTRACK__BEAT__SCORE_ACCUMULATOR_HPP