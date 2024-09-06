#include "implot.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GL/gl.h>
#include <fmt/core.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <random>
#include <vector>

// Helper function for creating a new ImGui frame for OpenGL.
void inline ImGui_NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

// Shared data.
struct ImPlotData {
  // Random number generator.
  std::mt19937 rand_gen;
  std::uniform_real_distribution<double> rand_uniform;

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

  // Helper function for generating a random floating point number.
  double rand_double() {
    return this->rand_uniform(this->rand_gen);
  }

  // Default constructor.
  ImPlotData(): rand_gen( std::random_device{}() ), rand_uniform(0.0, 1.0) {}
};

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

int main() {
  // Initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  const char* glsl_version = "#version 130";

  GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello world", nullptr, nullptr);
  if (window == nullptr) {
    spdlog::error("Failed to create GLFW window!");
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // vsync

  // DEBUG: Log the OpenGL version used.
  const GLubyte* opengl_version = glGetString(GL_VERSION);
  spdlog::info("Initialized GLFW with OpenGL Version: {}", reinterpret_cast<const char*>(opengl_version));

  // Setup ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Configure ImGui for OpenGL backends.
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Dark mode imgui!
  ImGui::StyleColorsDark();

  // Background color.
  ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

  // Create shared data struct to be passed into ImPlot.
  ImPlotData implot_data;

  // TEST: some test data.
  std::vector<double> x_data;
  std::vector<double> y_data;
  std::vector<double> bar_data;
  std::vector<double> scatter_data;

  // Randomize data.
  for (size_t i =0; i < 11; i++) {
    bar_data.push_back( implot_data.rand_double() * 10 );
    scatter_data.push_back( implot_data.rand_double() * 10 );
  }
  for (size_t i =0; i < 1000; i++) {
    x_data.push_back( i / 1000.0 * 9.0 );
    y_data.push_back( i / 1000.0 * 9.0 );
  }

  // Insert generated data into shared implot struct to be drawn.
  implot_data.bars.emplace_back("Bar Data", std::move(bar_data));
  implot_data.lines.emplace_back("Line Data", std::move(x_data), std::move(y_data), x_data.size());
  implot_data.points.emplace_back("Point Data", std::move(scatter_data));

  // Now we begin doing important stuff!
  // we now.. enter the draw loop!
  while( !glfwWindowShouldClose(window) ) {
    // Event poller.
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }


    // Draw stuff.
    draw_implot("1st plot", implot_data);


    // Actually draw ImGui
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Clean up.
  spdlog::info("Cleaning up...");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  ImPlot::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

