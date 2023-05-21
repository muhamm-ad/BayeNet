# Bayesian Network Inference Library

This library provides efficient and easy-to-use implementation for managing Bayesian Networks and executing various types of inferences.

## Features

This library features the implementation of the following types of inferences on Bayesian networks:

* Exact Inference
* Approximate Inference
* Gibbs Inference

## Dependencies

You'll need to have a modern C++ compiler (supporting C++17) and cmake (version 3.5 or newer) installed on your system. We also use Google Test for our testing.

You can install the dependencies on a Ubuntu system using the following command:

```bash
sudo apt-get install cmake libgtest-dev
```

For other operating systems, please check the official installation instructions of the respective dependencies.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Cloning the Repository

To clone this repository, run the following command in your terminal:

```bash
git clone https://github.com/muhamm-ad/BayeNet.git
```

### Building the Project

Navigate to the root directory of the project, and run the provided bash script `run.sh`. It automates the process of creating a build directory, running cmake and make.

```bash
./run.sh
```

The resulting executable and the test binaries will be in the `build` directory.

## Testing

After building the project, you can run the tests for each class separately.

To run the tests for the Bayesian Network class, execute:

```bash
./build/tests_BayeNet
```

To run the tests for the Variable class, execute:

```bash
./build/tests_Variable
```

You can also create your own tests by adding them to the respective test source files located in the `test` directory. Follow the same structure as the existing tests. After adding new tests, rebuild the project using the `run.sh` script and run the test binaries as described above.

## Usage

Include the library in your project, create instances of the `BayesianNetwork` class, add variables and dependencies to the network, and use the inference methods as needed.

## Contributing

Contributions to improve the library are welcome. Please feel free to open issues or submit pull requests.

## Built With

* [CMake](https://cmake.org/)
* [Google Test](https://github.com/google/googletest)

## Authors

* [muhamm-ad · GitHub](https://github.com/muhamm-ad)


