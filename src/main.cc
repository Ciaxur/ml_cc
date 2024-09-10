#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <imgui.h>
#include <implot.h>
#include <spdlog/spdlog.h>
#include "spdlog/details/os.h"
#include <thread>

#include "includes/plotting.h"
#include "includes/loss.h"

// TODO: remove this hardcoding of 2.
void forward(const double xs[][2], const double ws[], double ys[], const size_t sample_size) {
  // TODO: Activation operation (start with sigmoid)
  //   for now, just doing an identity. we'll improve after to learn WHY!
  for (size_t i = 0; i < sample_size; i++) {
    double x0 = xs[i][0];
    double w0 = ws[0];
    double n0 = w0 * x0;
    ys[i] = n0;

    spdlog::debug("== Input[{}] ==", i);
    spdlog::debug("  x0 * w0 = n0");
    spdlog::debug("  {} * {} = {}", x0, w0, n0);
    spdlog::debug("  n0 = {:.4}\n", n0);
  }
}

void train(const double xs[][2], double ws[], double ys[], const size_t sample_size, const double learning_rate, const size_t epoc) {
  for (size_t j = 0; j < epoc; j++) {
    for (size_t i = 0; i < sample_size; i++) {
      // ky = expected output
      // ka = actual output
      const double ky = xs[i][1];
      const double ka = ys[i];
      const double dy = Loss::d_mean_square_error( ky, ka, sample_size  );
      const double dy_lr = dy * learning_rate;

      spdlog::debug("== EPOC[{}]: TRAIN[{}] ==", j, i);
      spdlog::debug("  ky = {}", ky);
      spdlog::debug("  ka = {}", ka);
      spdlog::debug("  dy = {}", dy);
      spdlog::debug("  dy * l_rate = {}\n", dy_lr);

      // TODO: make this not hardcoded.
      ws[0] += dy_lr;
    }

    // Forward through the model and evaluate the performance of the model.
    forward(xs, ws, ys, sample_size);

    // Calculate the score after each epoc.
    double score = Loss::mean_square_error(
      xs,
      sample_size,
      ys,
      sample_size
    );
    spdlog::debug("epoc[{}] score: {:.4}", j, score);
  }
}

// NOTE: phase0 is verbose. learning what's going on.
// TODO: later.
// WIP: Training data: Bitwise operations
//
void do_ml_stuff(bool& should_close, ImPlotData& implot_data) {
  // Knobs.
  const double LEARNING_RATE = std::exp(-6);
  const size_t TRAIN_EPOC = 1000;

  // WIP: get something working first.
  // This bascially should be y = 2x
  const double TRAIN_DATA_LINEAR_OP[][2] = {
    // Input | Result
    {  0.0,      0.0 },
    {  1.0,      2.0 },
    {  2.0,      4.0 },
    {  3.0,      6.0 },
    {  4.0,      8.0 },
  };
  const size_t TRAIN_DATA_SET_SIZE = sizeof(TRAIN_DATA_LINEAR_OP) / sizeof(TRAIN_DATA_LINEAR_OP[0]);

  // WIP: Neuron(s) | Neural network
  double ws[] = {
    // Randomly populate this neuron's weights.
    // These weights must match each input.
    implot_data.rand_double(),
  };
  spdlog::info("w = {{ {:.4} }}", ws[0]);
  spdlog::info("learning rate = {:.4}", LEARNING_RATE);
  spdlog::info("epoc = {:}", TRAIN_EPOC);

  // Apply weighted sum operation on all training data sets
  double ys[TRAIN_DATA_SET_SIZE] = {};

  // Forward inputs through the model.
  forward(TRAIN_DATA_LINEAR_OP, ws, ys, TRAIN_DATA_SET_SIZE);

  // Evaluate score, which is using a loss function.
  double score = Loss::mean_square_error(
    // Evaluating training data set | expected outputs
    TRAIN_DATA_LINEAR_OP,
    TRAIN_DATA_SET_SIZE,

    // On actual output of the model
    ys,
    TRAIN_DATA_SET_SIZE
  );
  spdlog::info("score = {:.4}", score);

  // TRAIN: Adjust weights to reduce loss function.
  train( TRAIN_DATA_LINEAR_OP, ws, ys, TRAIN_DATA_SET_SIZE, LEARNING_RATE, TRAIN_EPOC );

  // EVALUATE
  // Forward through the model and evaluate the performance of the model.
  forward(TRAIN_DATA_LINEAR_OP, ws, ys, TRAIN_DATA_SET_SIZE);

  // Calculate the score after each epoc.
  score = Loss::mean_square_error(
    TRAIN_DATA_LINEAR_OP,
    TRAIN_DATA_SET_SIZE,
    ys,
    TRAIN_DATA_SET_SIZE
  );
  spdlog::info("epoc[{}] score: {:.4}", TRAIN_EPOC, score);

  // TODO: graph it. then after that we can play around with activation functions.
  // TEST:
  return;

  // Insert initial empty set of data.
  // implot_data.bars.emplace_back("Bar Data");
  // implot_data.lines.emplace_back("Line Data");
  // implot_data.points.emplace_back("Point Data");
  //
  // Grab a reference to the data arrays for dynamic mutation.
  // TODO:




  // Mutate the data plots.
  while (!should_close) {
    std::this_thread::sleep_for( std::chrono::milliseconds(50) );
  }
}

void configure_loglevel() {
  const std::string spdlog_level = spdlog::details::os::getenv("SPDLOG_LEVEL");

  if (spdlog_level == "info") {
    spdlog::set_level(spdlog::level::info);
  }
  else if (spdlog_level == "debug") {
    spdlog::set_level(spdlog::level::debug);
  }
  else if (spdlog_level == "warn") {
    spdlog::set_level(spdlog::level::warn);
  }
  else if (spdlog_level == "err") {
    spdlog::set_level(spdlog::level::err);
  }
  else if (spdlog_level == "trace") {
    spdlog::set_level(spdlog::level::trace);
  }
}

int main() {
  configure_loglevel();

  // TEST:
  ImPlotData implot_data_1;
  bool b = true;
  do_ml_stuff(b, implot_data_1);
  return 69;


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

