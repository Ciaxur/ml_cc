/**
 * This file include loss/cost functions/operations.
 */
#pragma once

#include <cstddef>

// TODO: remove the hardcoded [2] in the array.
namespace Loss {
  /**
   *
   */
  double mean_square_error(const double inputs[][2], size_t input_size, const double outputs[], size_t output_size);

  /**
   * Derivative of the cost function. This is used to get the slope towards
   * cost(x) = 0.
   *
   * @param expected_output Expected output from the model.
   * @param actual_output Actual output from the model.
   * @param sample_size Number of samples.
   *
   * @returns Result of the change in the mean_square_error over the change in
   *  the difference between actual and expected.
   */
  double d_mean_square_error(const double expected_output, const double actual_output, const size_t sample_size);
};
