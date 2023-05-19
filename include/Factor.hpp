#pragma once

#include "Variable.hpp"
#include <assert.h>
#include <functional>
#include <iostream>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

/**
 * @class Factor
 * @brief Represents a factor in a factor graph or a Bayesian network.
 *
 * A factor is a multi-dimensional table that contains the probabilities
 * of different configurations of a set of variables. The Factor class
 * provides methods to set and get the probabilities, as well as to
 * check if a variable is part of the factor.
 */
class Factor {
  public:
    /**
     * @brief Construct a new Factor with a set of variables.
     * @param variables Vector of shared pointers to Variable objects.
     */
    Factor(std::vector<std::shared_ptr<Variable>> vars);

    /**
     * @brief Get the variables associated with the factor.
     * @return Vector of shared pointers to Variable objects.
     */
    std::vector<std::shared_ptr<Variable>> getVariables() const;

    /**
     * @brief Get the probabilities associated with the factor.
     * @return Vector of doubles representing probabilities.
     */
    std::vector<double> getProbabilities() const;

    /**
     * @brief Check if the factor has the given variable.
     * @param variable Shared pointer to a Variable object.
     * @return true if the factor has the variable, false otherwise.
     */
    bool hasVariable(const std::shared_ptr<Variable> &variable) const;

    /**
     * @brief Set the probability for a given configuration of values.
     * @param values Vector of boolean values representing a configuration.
     * @param probability Double representing the probability of the configuration.
     */
    void setProbability(const std::vector<bool> &values, double probability);

    /**
     * @brief Get the probability for a given configuration of values.
     * @param values Vector of boolean values representing a configuration.
     * @return Double representing the probability of the configuration.
     */
    double getProbability(const std::vector<bool> &values) const;

    /**
     * @brief Print the factor and its associated probabilities.
     */
    void print() const;

  private:
    std::vector<std::shared_ptr<Variable>> variables; // Variables associated with the factor
    std::vector<double> probabilities;                // Probabilities for different configurations
};
