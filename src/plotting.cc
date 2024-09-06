#include <fmt/core.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <implot.h>

#include "includes/plotting.h"

void inline ImGui_NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void inline ImGui_FullScreenNextWindow() {
  // Set the imgui window size to cover entire window.
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::SetNextWindowSize(io.DisplaySize);
  ImGui::SetNextWindowPos(ImVec2(0, 0));
}

void draw_implot(const char* plot_name, ImPlotData& implot_data) {
  ImGuiIO& io = ImGui::GetIO();

  ImGui_NewFrame();

  // Set the imgui window size to cover entire window.
  ImGui_FullScreenNextWindow();

  // Define ImGui stuff
  ImGui::Begin(
    fmt::format("Fullscreen {}", plot_name).c_str(),
    nullptr,
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
  );

  ImPlot::BeginPlot(
    plot_name,
    io.DisplaySize,
    ImPlotFlags_::ImPlotFlags_NoTitle
  );

  // Draw the data!
  for (size_t i = 0; i < implot_data.bars.size(); i++) {
    const char* name           = implot_data.lines[i].name;
    const double* bar_data_ptr = implot_data.bars[i].data.data();
    const size_t bar_data_size = implot_data.bars[i].data.size();

    ImPlot::PlotBars(
      name,
      bar_data_ptr,
      bar_data_size
    );
  }

  for (size_t i = 0; i < implot_data.lines.size(); i++) {
    const char* name              = implot_data.lines[i].name;
    const double* line_x_data_ptr = implot_data.lines[i].x_data.data();
    const double* line_y_data_ptr = implot_data.lines[i].y_data.data();
    const size_t line_data_size   = implot_data.lines[i].size;

    ImPlot::PlotLine(
      name,
      line_x_data_ptr,
      line_y_data_ptr,
      line_data_size
    );
  }

  // Draw points on top of lines
  for (size_t i = 0; i < implot_data.points.size(); i++) {
    const char* name           = implot_data.points[i].name;
    const double* point_data_ptr = implot_data.points[i].data.data();
    const size_t point_data_size = implot_data.points[i].data.size();

    ImPlot::PlotScatter(
      name,
      point_data_ptr,
      point_data_size
    );
  }


  ImPlot::EndPlot();

  ImGui::End();
  ImGui::Render();
}
