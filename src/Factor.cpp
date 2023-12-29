#include "../include/Factor.hpp"
#include <cmath>
#include <utility>

// Construct a new Factor with a set of variables.
Factor::Factor(std::vector<std::shared_ptr<Variable>> vars) : variables(std::move(vars)) {}

// Get the variables associated with the factor.
std::vector<std::shared_ptr<Variable>> Factor::getVariables() const { return variables; }

// Get the probabilities associated with the factor.
std::vector<double> Factor::getProbabilities() const { return probabilities; }

// Check if the factor has the given variable.
bool Factor::hasVariable(const std::shared_ptr<Variable> &variable) const {
    for (const auto& v : variables)
        if (v == variable)
            return true;
    return false;
}

// Set the probability for a given configuration of values.
void Factor::setProbability(const std::vector<bool> &values, double probability) {
    // assert(values.size() == variables.size());
    unsigned int index = 0;
    for (unsigned int pos = 0; pos < values.size(); pos++) {
        index += pow(2, pos) * values[pos];
    }

    if (index >= probabilities.size()) {
        probabilities.resize(index + 1);
    }
    probabilities[index] = probability;
}

// Get the probability for a given configuration of values.
double Factor::getProbability(const std::vector<bool> &values) const {
    // assert(values.size() == variables.size());
    unsigned int index = 0;
    for (unsigned int pos = 0; pos < values.size(); pos++) {
        index += pow(2, pos) * values[pos];
    }
    return probabilities.at(index);
}

// Print the factor and its associated probabilities.
void Factor::print() const {
    std::cout << "Factor(";
    for (const auto &variable : variables) {
        std::cout << "Variable(" << variable->getName() << "), ";
    }
    std::cout << ")Probabilities: ";
    for (const auto &probability : probabilities) {
        std::cout << probability << ", ";
    }
    std::cout << "\n";
}