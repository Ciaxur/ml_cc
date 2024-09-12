/**
 * This file include loss/cost functions/operations.
 */
#pragma once

#include <cstddef>

namespace Loss {
  /**
   * Evaluates the Mean Squared Error on the given data.
   *
   * @param expected_ys Expected outputs
   * @param actual_ys Actual outputs
   * @param sample_size Size of the array.
   *
   * @returns the result of running MSE.
   */
  double mean_square_error(const double expected_ys[], const double actual_ys[], const size_t sample_size);

  /**
   * Derivative of the cost function. This is used to get the slope towards
   * cost(x) = 0.
   *
   * @param expected_y Expected output from the model.
   * @param actual_y Actual output from the model.
   * @param sample_size Size of the array.
   *
   * @returns Result of the change in the mean_square_error over the change in
   *  the difference between actual and expected.
   */
  double d_mean_square_error(const double expected_y, const double actual_y, const size_t sample_size);
};
