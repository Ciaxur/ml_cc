#include "includes/model.h"


Neuron::Neuron(size_t input_size) {
  this->weights = new double[input_size];
  this->_w_size = input_size;
};


Neuron::~Neuron() {
  if (this->weights != nullptr) delete[] this->weights;
  this->_w_size = 0;
}

size_t Neuron::size() const {
  return this->_w_size;
}

