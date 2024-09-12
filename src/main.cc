#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <functional>
#include <imgui.h>
#include <implot.h>
#include <spdlog/spdlog.h>
#include "includes/model.h"
#include "spdlog/details/os.h"
#include <thread>

#include "includes/plotting.h"
#include "includes/loss.h"

template<typename T>
std::string array_to_str(const T vec[], const size_t size) {
  std::string str;

  str += fmt::format("[ ");
  for (size_t i = 0; i < size; i++) {
    const std::string separator = i + 1 == size ? " " : ", ";
    str += fmt::format("{}{}", vec[i], separator);
  }
  str += "]";

  return str;
}

/**
* Forwards the given input through the model, populating outputs.
*
* @param xs Input array to feed throught the model.
* @param ws Model weights.
* @param ys Output array to populate.
* @param sample_size Size of the input sample array.
*/
void forward(const double xs[], const double ws[], double ys[], const size_t sample_size) {
  // TODO: Activation operation (start with sigmoid)
  //   for now, just doing an identity. we'll improve after to learn WHY!
  for (size_t i = 0; i < sample_size; i++) {
    double x0 = xs[i];
    // TODO: currently, this is hardcoded to a single neuron.
    double w0 = ws[0];
    double n0 = w0 * x0;
    ys[i] = n0;

    spdlog::debug("== Input[{}] ==", i);
    spdlog::debug("  x0 * w0 = n0");
    spdlog::debug("  {} * {} = {}", x0, w0, n0);
    spdlog::debug("  n0 = {:.4}\n", n0);
  }
}

/**
* Trains the given model's weights given the expected input and outputs.
*
* @param xs Input training data.
* @param expected_ys Expected output training data.
* @param ws Model weights to train.
* @param sample_size Size of the training sample dataset.
* @param learning_rate Learning rate of the model.
* @param epoc Number of epocs to iterate through training.
*/
void train(const double xs[], const double expected_ys[], const size_t sample_size, double ws[] , const double learning_rate, const size_t epoc) {
  // Temporarily create outputs to store for training.
  double *ys = new double[sample_size];

  // Initial evaluation.
  forward(xs, ws, ys, sample_size);

  // Training begins.
  for (size_t j = 0; j < epoc; j++) {

    // WEIGHT ADJUSTMENT
    for (size_t i = 0; i < sample_size; i++) {
      // ky = expected output
      // ka = actual output
      const double ky = expected_ys[i];
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

    // EVALUATION
    // Forward through the model and evaluate the performance of the model.
    forward(xs, ws, ys, sample_size);

    // MODEL PERFORMANCE
    // Calculate the score after each epoc.
    double score = Loss::mean_square_error(
      // Evaluating training dataset | expected outputs
      expected_ys,

      // On actual outputs of the model
      ys,
      sample_size
    );
    spdlog::debug("epoc[{}] score: {:.4}", j, score);

  }

  // Clean up.
  delete[] ys;
}

// NOTE: phase0 is verbose. learning what's going on.
// TODO: later.
void do_ml_stuff(bool& should_close, ImPlotData& implot_data) {
  Model model{
    .learning_rate = std::exp(-6),
    .training_epochs = 1,
    .cost = 69,

    // Single neuron.
    .n = { 1 },
  };

  // Add model to ImPlot's shared data.
  implot_data.model = &model;


  // WIP: get something working first.
  // This bascially should be y = 2x
  const double TRAIN_XS_DATA_LINEAR_OP[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
  const double TRAIN_YS_DATA_LINEAR_OP[] = { 0.0, 2.0, 4.0, 6.0, 8.0 };
  const size_t TRAIN_DATA_SET_SIZE = sizeof(TRAIN_XS_DATA_LINEAR_OP) / sizeof(TRAIN_XS_DATA_LINEAR_OP[0]);

  // Neuron(s) | Neural network
  // Randomly populate this neuron's weights.
  // These weights must match each input.
  for (size_t i = 0; i < model.n.size(); i++) {
    model.n.weights[i] = implot_data.rand_double();
  }

  spdlog::info("======= Model =======");
  spdlog::info("w = {{ {:.4} }}", model.n.weights[0]);
  spdlog::info("learning rate = {:.4}", model.learning_rate);
  spdlog::info("epoch = {}", model.training_epochs);

  // Apply weighted sum operation on all training datasets
  double training_ys[TRAIN_DATA_SET_SIZE] = {};
  forward(TRAIN_XS_DATA_LINEAR_OP, model.n.weights, training_ys, TRAIN_DATA_SET_SIZE);

  // Evaluate score, which is using a loss function.
  double score = Loss::mean_square_error(
    // Evaluating training dataset | expected outputs
    TRAIN_YS_DATA_LINEAR_OP,

    // On actual output of the model
    training_ys,
    TRAIN_DATA_SET_SIZE
  );
  spdlog::info("score = {:.4}", score);

  // Insert initial empty set of data.
  //
  // implot_data.bars.emplace_back("Bar Data");
  implot_data.lines.emplace_back("Model function");
  implot_data.points.emplace_back("training xs");
  implot_data.points.emplace_back("actual xs");

  // Grab references to data in the array.
  ImPlotData::ScatterData &training_xs_points = implot_data.points[0];
  ImPlotData::ScatterData &xs_points          = implot_data.points[1];
  ImPlotData::LineData &model_lines           = implot_data.lines[0];

  // Generate data.
  const size_t data_sample_size = 10;
  double xs[data_sample_size]   = {0};
  double ys[data_sample_size]   = {0};
  for (size_t i = 0; i < data_sample_size; i++) xs[i] = i;

  // Populate training data.
  training_xs_points.data.resize(TRAIN_DATA_SET_SIZE);
  for (size_t i = 0; i < TRAIN_DATA_SET_SIZE; i++) {
    training_xs_points.data[i] = TRAIN_YS_DATA_LINEAR_OP[i];
  }

  // Apply model on the data.
  forward(xs, model.n.weights, ys, data_sample_size);
  spdlog::info("Sample size: {} ", data_sample_size);
  spdlog::info("Inputs:      {} ", array_to_str(xs, data_sample_size));
  spdlog::info("Weights:     {} ", array_to_str(model.n.weights, model.n.size()));
  spdlog::info("Outputs:     {} ", array_to_str(ys, data_sample_size));

  // Pre-allocate space for data to plot.
  xs_points.data.resize(data_sample_size);
  model_lines.x_data.resize(data_sample_size);
  model_lines.y_data.resize(data_sample_size);
  model_lines.size = data_sample_size;

  // Populate data into plot.
  for (size_t i = 0; i < data_sample_size; i++) {
    model_lines.x_data[i] = xs[i];
    model_lines.y_data[i] = ys[i];
    xs_points.data[i] = ys[i];
  }

  // Mutate the data plots.
  while (!should_close) {
    std::this_thread::sleep_for( std::chrono::milliseconds(50) );

    // TRAIN: Adjust weights to reduce loss function and improve the accuracy of
    // the model.
    train(
      TRAIN_XS_DATA_LINEAR_OP,
      TRAIN_YS_DATA_LINEAR_OP,
      TRAIN_DATA_SET_SIZE,
      model.n.weights,
      model.learning_rate,
      model.training_epochs
    );

    // Forward data through the model.
    forward(xs, model.n.weights, ys, data_sample_size);

    // Populate data into plot.
    for (size_t i = 0; i < data_sample_size; i++) {
      model_lines.x_data[i] = xs[i];
      model_lines.y_data[i] = ys[i];
      xs_points.data[i] = ys[i];
    }

    // Evaluate model's performance.
    model.cost = Loss::mean_square_error(
      // Evaluating training dataset | expected outputs
      TRAIN_YS_DATA_LINEAR_OP,

      // On actual output of the model
      ys,
      TRAIN_DATA_SET_SIZE
    );

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

