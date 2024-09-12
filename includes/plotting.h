#pragma once

#include <random>
#include "includes/model.h"

/**
* Helper function for creating a new ImGui frame for OpenGL.
*/
void inline ImGui_NewFrame();

/**
* Helper function for fullscreening the next drawn window.
*/
void inline ImGui_FullScreenNextWindow();

// Plotting data structure given to a single ImPlot draw call.
struct ImPlotData {
  // Random number generator.
  std::mt19937 rand_gen;
  std::uniform_real_distribution<double> rand_uniform;

  // Model metadata.
  Model *model = nullptr;

  // Data types.
  struct BarsData {
    const char* name;
    std::vector<double> data;
  };

  struct LineData {
    const char* name;
    std::vector<double> x_data;
    std::vector<double> y_data;
    size_t size;
  };

  struct ScatterData {
    const char* name;
    std::vector<double> data;
  };

  // Data to draw.
  std::vector<BarsData> bars;
  std::vector<LineData> lines;
  std::vector<ScatterData> points;

  // Helper function for generating a random floating point number between
  // 0.0 - 1.0.
  double rand_double() {
    return this->rand_uniform(this->rand_gen);
  }

  // Default constructor.
  ImPlotData(): rand_gen( std::random_device{}() ), rand_uniform(0.0, 1.0) {}
};

/**
* Draws a plot of the given ImPlotData structure.
*
* @param plot_name Unique name for the ImPlot window.
* @param implot_data Data structure used to draw plots of.
*/
void draw_implot(const char* plot_name, ImPlotData& implot_data);
