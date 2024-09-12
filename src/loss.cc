#include <spdlog/spdlog.h>
#include "includes/loss.h"

double Loss::mean_square_error(const double expected_ys[], const double actual_ys[], size_t sample_size) {
  // Mean squared error.
  double mse = 0;
  for (size_t i = 0; i < sample_size; i++) {
    const double expected_y = expected_ys[i];
    const double actual_y   = actual_ys[i];

    double distance_from_expected_result = actual_y - expected_y;

    // Square it: remove negatives while increasing the "loudness" of error.
    distance_from_expected_result *= distance_from_expected_result;

    spdlog::debug("MSE[{}]: expected={:.4} | actual={:.4} | diff_squared={:.4}", i, expected_y, actual_y, distance_from_expected_result);
    mse += distance_from_expected_result;
  }

  // Divide by the total iterations to get the average.
  return mse / sample_size;
}

double Loss::d_mean_square_error(const double expected_y, const double actual_y, const size_t sample_size) {
  // Derivative of MSE.
  // lim of MSE as the output approaches 0. Which is the derivative.
  //
  // d_mse = 2 * ( yk - ya ) / n
  //   for yk = expected output
  //       ya = actual output
  //       n  = sample size
  return ( 2 * ( expected_y - actual_y ) ) / sample_size;
}

