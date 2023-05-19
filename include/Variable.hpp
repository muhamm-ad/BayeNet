#pragma once

#include "../lib/prettyprint.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <vector>

/**
 * @class Variable
 * @brief Represents a variable in a Bayesian Network.
 */
class Variable {
    std::string name;                               ///< Name of the variable
    std::vector<std::shared_ptr<Variable>> parents; ///< Parent variables of the current variable
    std::vector<Variable *> children;               ///< Children variables of the current variable
    std::vector<double> probTable;                  ///< Probability table for the current variable
    bool val;                                       ///< Value of the current variable

  public:
    /**
     * @brief Constructor for Variable.
     * @param vec Vector of shared_ptr to parent variables
     * @param name Name of the variable
     */
    Variable(const std::string &name, std::vector<std::shared_ptr<Variable>> vec);

    /**
     * @brief Get the name of the variable.
     * @return The name of the variable (string).
     */
    const std::string &getName() const;

    /**
     * @brief Set the name of the variable.
     * @param name The name to set (string).
     */
    void setName(const std::string &name);

    /**
     * @brief Get the parent variables.
     * @return A const reference to the vector of parent variables.
     */
    const std::vector<std::shared_ptr<Variable>> &getParents() const;

    /**
     * @brief Get the child variables.
     * @return A const reference to the vector of child variables.
     */
    const std::vector<Variable *> &getChildren() const;

    /**
     * @brief Set the value of the current variable.
     * @param value The value to be set (boolean).
     */
    void setVal(bool value);

    /**
     * @brief Get the value of the current variable.
     * @return The value of the current variable (boolean).
     */
    bool getVal() const;

    /**
     * @brief Set the probabilities in the table using variadic templates.
     * @tparam Args Variadic template arguments.
     */
    template <typename... Args> void setProb(Args... args) { setProb_REC(0, 0, args...); }

    /**
     * @brief Set the probabilities in the table using a vector of parent values.
     * @tparam Args Variadic template arguments.
     * @param parentValues A vector of parent values.
     * @param p The probability to set.
     */
    template <typename... Args> void setProb(std::vector<bool> parentValues, double p) {
        unsigned int index = 0;
        for (unsigned int pos = 0; pos < parentValues.size(); pos++) {
            index += pow(2, pos) * parentValues[pos];
        }

        // Resize the probability table if needed
        if (index >= probTable.size()) {
            probTable.resize(index + 1);
        }
        probTable[index] = p;
    }

    /**
     * @brief Access the probability in the table using variadic templates.
     * @tparam Args Variadic template arguments.
     * @return The probability value.
     */
    template <typename... Args> double getProb(Args... args) {
        return probTable.at(POSITION(0, 0, args...));
    }

    /**
     * @brief Access the probability in the table using a vector of parent values.
     * @param parentValues A vector of parent values.
     * @return The probability value.
     */
    double getProb(std::vector<bool> parentValues);

    /**
     * @brief Sample a value for the current variable based on its probability table.
     * @return The sampled value (boolean).
     */
    bool sample();

    /**
     * @brief Sample a value for the current variable based on its probability table and a given
     * parent sample.
     * @param parent_sample A map containing the parent sample.
     * @return The sampled value (boolean).
     */
    bool sample(const std::map<std::shared_ptr<Variable>, bool> &parent_sample);

    /**
     * @brief Sample a value for the current variable based on its probability table and a given
     * Markov blanket.
     * @param markovBlanket A vector representing the Markov blanket.
     * @return The sampled value (boolean).
     */
    bool sample(const std::vector<bool> &markovBlanket);

  private:
    /**
     * @brief Calculate the probability of the variable being true given the mixed Markov blanket.
     * @param mixedMarkovBlanket Vector of boolean values representing the mixed Markov blanket.
     * @return The probability value.
     */
    double getProb_mixedMarkovBlanket(const std::vector<bool> &mixedMarkovBlanket);

    /**
     * @brief Recursive helper function to set probabilities in the table.
     * @tparam Args Variadic template arguments.
     * @param index Index in the probability table.
     * @param pos Position in the probability table.
     * @param val Value to set in the probability table.
     */
    template <typename... Args> void setProb_REC(int index, int pos, bool val, Args... args) {
        setProb_REC(index + pow(2, pos) * val, pos + 1, args...);
    }

    /**
     * @brief Recursive helper function to set probabilities in the table.
     * @tparam Args Variadic template arguments.
     * @param index Index in the probability table.
     * @param pos Position in the probability table.
     * @param val Value to set in the probability table.
     */
    template <typename... Args> void setProb_REC(int index, int pos, double val) {
        if (unsigned(index) >= probTable.size()) {
            probTable.resize(index + 1);
        }
        probTable[index] = val;
    }

    /**
     * @brief Calculate the position of a given value in the probability table.
     * @tparam Args Variadic template arguments.
     * @param index Index in the probability table.
     * @param pos Position in the probability table.
     * @param val Value to set in the probability table.
     * @return The position of the given value in the probability table.
     */
    template <typename... Args> unsigned int POSITION(int index, int pos, bool val, Args... args) {
        return POSITION(index + pow(2, pos) * val, pos + 1, args...);
    }

    /**
     * @brief Calculate the position of a given value in the probability table.
     * @tparam Args Variadic template arguments.
     * @param index Index in the probability table.
     * @param pos Position in the probability table.
     * @return The position of the given value in the probability table.
     */
    template <typename... Args> unsigned int POSITION(int index, int pos) { return index; }
};
