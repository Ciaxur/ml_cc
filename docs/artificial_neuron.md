# Artificial Neuron

An artificial neuron is the basic building block for a neural network. It
basically gets `N inputs`, applies `weights` to the `inputs`, and sums them to
produce an `output`. This is called the **weighted sum**.

There's also the idea of `bias`, which is added to the result of the `inputs`
such that it shifts the function towards the training data.


## Activation function
Serves kind of like normalization of the data. These types of functions depend on
the kind of data we're working with. For example, one could use a `step function`
which results in the output claming to either `1` or `0`. If there is no actual input
reason to have other kinds of outputs, this would be a boolean.


TODO: why are there different ones that just the sum of the weighted inputs?
- Apprarently this is mostly useful in the first layer. But why?
- Interesting. It also allows to minimize computation. Like sigmoid.


TODO: but why? what do the various functions' pros and cons? why not just one?

## Loss/Cost function
TODO: Mean Squared Error. Why? how it work?


One can use Finite Difference as a dirivative approximation (not recommended though | there are better ways).
- https://en.wikipedia.org/wiki/Finite_difference


# Resources
- https://en.wikipedia.org/wiki/Artificial_neuron
- https://en.wikipedia.org/wiki/Activation_function
- https://en.wikipedia.org/wiki/Neural_network_(machine_learning)
- https://en.wikipedia.org/wiki/Loss_function
  - https://en.wikipedia.org/wiki/Mean_squared_error

