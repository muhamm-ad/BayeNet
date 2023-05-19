#include "../include/Variable.hpp"
#include <cmath>
#include <random>

// Constructor implementation
Variable::Variable(const std::string &name, std::vector<std::shared_ptr<Variable>> vec)
    : name(name), parents(vec) {
    // Connect this variable to its parents' children
    for (auto &e : parents) {
        e->children.push_back(this);
    }
}

// Get the name of the variable
const std::string &Variable::getName() const { return name; }

// Set the name of the variable
void Variable::setName(const std::string &name) { this->name = name; }

// Get the parent variables
const std::vector<std::shared_ptr<Variable>> &Variable::getParents() const { return parents; }

// Get the child variables
const std::vector<Variable *> &Variable::getChildren() const { return children; }

// Set the value of the variable
void Variable::setVal(bool value) { val = value; }

// Get the value of the variable
bool Variable::getVal() const { return val; }

// Access the probability in the probability table based on parent values
double Variable::getProb(std::vector<bool> parentValues) {
    unsigned int index = 0;
    for (unsigned int pos = 0; pos < parentValues.size(); pos++) {
        index += pow(2, pos) * static_cast<int>(parentValues[pos]);
    }

    try {
        return probTable.at(index);
    } catch (const std::out_of_range &e) {
        std::cerr << "Caught an out_of_range exception: " << e.what() << '\n';
        return 0.0; // return 0 as a fallback
    }
}

// Sample a new value for the variable based on its probability table and parent values
bool Variable::sample() {
    // Initialize random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    double prob = 0;
    if (parents.empty()) {
        try {
            prob = probTable.at(0);
        } catch (const std::out_of_range &e) {
            std::cerr << "Caught an out_of_range exception: " << e.what() << '\n';
        }
    } else {
        std::vector<bool> parent_values;
        for (const auto &parent : parents) {
            parent_values.push_back(parent->getVal());
        }
        prob = getProb(parent_values);
    }

    // Return true if the random value is less than the probability, false otherwise
    return dis(gen) <= prob;
}

// Sample a new value for the variable given a specific set of parent values
bool Variable::sample(const std::map<std::shared_ptr<Variable>, bool> &parent_sample) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    double prob = 0.0;

    try {
        if (parents.empty()) {
            prob = probTable.at(0);
        } else {
            std::vector<bool> parent_values;
            for (const auto &parent : parents) {
                parent_values.push_back(parent_sample.at(parent));
            }
            prob = getProb(parent_values);
        }
    } catch (const std::out_of_range &e) {
        std::cerr << "Caught an out_of_range exception: " << e.what() << '\n';
        return false; // or handle the error in some other appropriate way
    }

    return dis(gen) <= prob;
}

// Sample a new value for the variable given a Markov blanket
bool Variable::sample(const std::vector<bool> &markovBlanket) {
    // Calculate the probability of the variable being true given the Markov blanket
    double probTrue = getProb_mixedMarkovBlanket(markovBlanket);

    // Sample a random value between 0 and 1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double randomValue = dis(gen);

    // Return true if the random value is less than the probability of being true, false otherwise
    return randomValue < probTrue;
}

// Calculate the probability of the variable being true given a mixed Markov blanket
double Variable::getProb_mixedMarkovBlanket(const std::vector<bool> &mixedMarkovBlanket) {
    std::vector<bool> parentValues;
    for (unsigned int i = 0; i < parents.size(); ++i) {
        parentValues.push_back(mixedMarkovBlanket[i]);
    }
    double probTrue = getProb(parentValues);

    for (unsigned int i = parents.size(); i < mixedMarkovBlanket.size(); ++i) {
        unsigned int childIndex = i - parents.size();
        std::vector<bool> childParentValues;
        for (auto it = mixedMarkovBlanket.begin() + parents.size();
             it != mixedMarkovBlanket.begin() + parents.size() +
                       children[childIndex]->getParents().size();
             ++it) {
            childParentValues.push_back(*it);
        }
        probTrue *= children[childIndex]->getProb(childParentValues);
    }

    return probTrue;
}
