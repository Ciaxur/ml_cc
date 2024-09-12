#pragma once
#include <cstddef>

class Neuron {
private:
  // Weights to apply to each input x.
  size_t _w_size;

public:
  Neuron(size_t input_size);
  ~Neuron();

  double *weights = nullptr;
  size_t size() const;
};


// TODO: wip. for now making this a struct, but later can generalize it
// into a class.
struct Model {
  // Metadata.
  double learning_rate;
  size_t training_epochs;
  double cost;

  // TODO: support multiple neurons & layers.
  Neuron n;
};
