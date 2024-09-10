#include <spdlog/spdlog.h>
#include "includes/loss.h"

double Loss::mean_square_error(const double inputs[][2], size_t input_size, const double outputs[], size_t sample_size) {
  // Mean squared error.
  double mse = 0;
  for (size_t i = 0; i < sample_size; i++) {
    const double expected_y = inputs[i][1];
    const double actual_y   = outputs[i];

    double distance_from_expected_result = actual_y - expected_y;

    // Square it: remove negatives while increasing the "loudness" of error.
    distance_from_expected_result *= distance_from_expected_result;

    spdlog::debug("MSE[{}]: expected={:.4} | actual={:.4} | diff_squared={:.4}", i, expected_y, actual_y, distance_from_expected_result);
    mse += distance_from_expected_result;
  }

  // Divide by the total iterations to get the average.
  return mse / sample_size;
}

double Loss::d_mean_square_error(const double expected_output, const double actual_output, const size_t sample_size) {
  // Derivative of MSE.
  // lim of MSE as the output approaches 0. Which is the derivative.
  //
  // d_mse = 2 * ( yk - ya ) / n
  //   for yk = expected output
  //       ya = actual output
  //       n  = sample size
  return ( 2 * ( expected_output - actual_output ) ) / sample_size;
}

