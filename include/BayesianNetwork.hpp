#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "Chrono.h"
#include "Variable.hpp"

template <class T> void addArgsToVector(std::vector<T> &vec) {}

template <class T, typename... Args>
void addArgsToVector(std::vector<T> &vec, T arg, Args... args) {
    vec.push_back(arg);
    addArgsToVector(vec, args...);
}

class BayesianNetwork {
    // vector that contains the random variables of the Bayesian network
    std::vector<std::shared_ptr<Variable>> variables;

  public:
    /**
     * Adds a Boolean random variable with the specified probability to
     * the Bayesian network.
     * @param varName: The name of the Boolean variable.
     * @param p: The probability of the Boolean variable being true.
     * @return: A shared pointer to the added Variable object.
     */
    std::shared_ptr<Variable> addBooleanVar(const std::string &varName = "", double p = 0);

    /**
     * Overloaded function to add a Boolean random variable with the specified
     *  arguments to the Bayesian network.
     * @param varName: The name of the Boolean variable.
     * @param args: A variadic template for arguments to construct the Boolean variable.
     * @return: A shared pointer to the added Variable object.
     */
    template <typename... Args>
    std::shared_ptr<Variable> addBooleanVar(const std::string &varName = "", Args... args) {
        std::vector<std::shared_ptr<Variable>> parents;
        addArgsToVector(parents, args...);
        variables.push_back(std::make_shared<Variable>(varName, parents));
        return variables.back();
    }

    /**
     * Retrieves a shared pointer to a Variable from the Bayesian network, based on its name.
     * @param varName: The name of the variable to retrieve.
     * @return: A shared pointer to the Variable if found, nullptr otherwise.
     */
    std::shared_ptr<Variable> getVar(const std::string &varName);

    /**
     * Prints the structure of the Bayesian network, including information about
     * each variable and their dependencies.
     */
    void print() const;

    /**
     * Prints the specified variables from the Bayesian network.
     * @param variables: A vector of shared pointers to the Variable objects to be printed.
     */
    void printVariables(const std::vector<std::shared_ptr<Variable>> &variables);

    /**
     * Performs exact inference on the Bayesian network given evidence,
     * estimating the probability distribution of the specified variables.
     * @param varsToEstimate: A vector of tuples containing shared pointers to the
     *                          Variable objects to be estimated and their boolean values.
     * @param evidence: A vector of tuples containing shared pointers to the Variable
     *                  objects representing the evidence and their boolean values.
     * @return: The estimated probability of the varsToEstimate given the evidence.
     */
    double
    exactInference(const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &varsToEstimate,
                   const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &evidence);

    /**
     * Performs approximate inference on the Bayesian network given evidence,
     * estimating the probability distribution of the specified variables using a
     * specified number of iterations.
     * @param varsToEstimate: A vector of tuples containing shared pointers to the
     *                          Variable objects to be estimated and their boolean values.
     * @param evidence: A vector of tuples containing shared pointers to the Variable
     *                  objects representing the evidence and their boolean values.
     * @param numIterations: The number of iterations to perform during the approximate inference.
     * @return: The estimated probability of the varsToEstimate given the evidence.
     */
    double
    approximateInference(std::vector<std::tuple<std::shared_ptr<Variable>, bool>> varsToEstimate,
                         std::vector<std::tuple<std::shared_ptr<Variable>, bool>> evidence,
                         unsigned int numIterations);

    /**
     * Performs Gibbs sampling inference on the Bayesian network given evidence,
     * estimating the probability distribution of the specified variables using
     * a specified number of iterations.
     * @param varsToEstimate: A vector of tuples containing shared pointers to the
     *                          Variable objects to be estimated and their boolean values.
     * @param evidence: A vector of tuples containing shared pointers to the
     *                  Variable objects representing the evidence and their boolean values.
     * @param numIterations: The number of iterations to perform during the Gibbs sampling
     * inference.
     * @return: The estimated probability of the varsToEstimate given the evidence.
     */
    double gibbsInference(std::vector<std::tuple<std::shared_ptr<Variable>, bool>> varsToEstimate,
                          std::vector<std::tuple<std::shared_ptr<Variable>, bool>> evidence,
                          unsigned int numIterations);

  private:
    // Add private member functions as needed
};