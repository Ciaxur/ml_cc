#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <chrono>
#include <fmt/core.h>
#include <functional>
#include <imgui.h>
#include <implot.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <vector>

#include "includes/plotting.h"

void do_ml_stuff(bool& should_close, ImPlotData& implot_data) {
  // Insert initial empty set of data.
  implot_data.bars.emplace_back("Bar Data");
  implot_data.lines.emplace_back("Line Data");
  implot_data.points.emplace_back("Point Data");

  // Grab a reference to the data arrays for dynamic mutation.
  ImPlotData::ScatterData& points = implot_data.points.back();
  points.data.resize(100);

  // Mutate the data plots.
  while (!should_close) {
    std::this_thread::sleep_for( std::chrono::milliseconds(1) );

    for (size_t i = 0; i < 100; i++) {
      points.data[i] = implot_data.rand_double() * 10;
    }
  }
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
  bool ml_worker_thread_should_close = false;
  std::thread ml_worker_thread( do_ml_stuff, std::ref(ml_worker_thread_should_close), std::ref(implot_data) );

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
    draw_implot("model plot", implot_data);

    // Actually draw ImGui
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Terminate worker thread
  spdlog::info("Waiting on worker thread to terminate...");
  ml_worker_thread_should_close = true;
  ml_worker_thread.join();

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

