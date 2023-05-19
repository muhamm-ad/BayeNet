#include "../include/BayesianNetwork.hpp"

//************************************************/
//************** PUBLIC FUNCTIONS ****************/
//************************************************/

// Add an independent boolean random variable to the Bayesian network
std::shared_ptr<Variable> BayesianNetwork::addBooleanVar(const std::string &varName, double p) {
    variables.push_back(
        std::make_shared<Variable>(varName, std::vector<std::shared_ptr<Variable>>()));
    variables.back()->setProb(p);
    return variables.back();
}

// Retrieves a shared pointer to a Variable from the Bayesian network, based on its name.
std::shared_ptr<Variable> BayesianNetwork::getVar(const std::string &varName) {
    for (const auto &variable : variables) {
        if (variable->getName() == varName) {
            return variable;
        }
    }
    return nullptr;
}

void BayesianNetwork::print() const {
    std::cout << "Bayesian Network:\n";
    for (const auto &var : variables) {
        std::cout << "Variable(" << var->getName() << "): ";
        for (const auto &parent : var->getParents()) {
            std::cout << "Variable(" << parent->getName() << "), ";
        }
        std::cout << "\n";
    }
}

void BayesianNetwork::printVariables(const std::vector<std::shared_ptr<Variable>> &variables) {
    std::cout << "Variables: ";
    for (const auto &var : variables) {
        std::cout << "Variable(" << var->getName() << "), ";
    }
    std::cout << "\n";
}

double BayesianNetwork::exactInference(
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &varsToEstimate,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &evidence) {

    // Chrono C("exactInference");
    // TODO : Exact evaluation

    return 1;
}

double BayesianNetwork::approximateInference(
    std::vector<std::tuple<std::shared_ptr<Variable>, bool>> varsToEstimate,
    std::vector<std::tuple<std::shared_ptr<Variable>, bool>> evidence, unsigned int numIterations) {

    // Chrono C("approximateInference");
    // TODO : Approximate evaluation by rejection sampling

    return 1;
}

double BayesianNetwork::gibbsInference(
    std::vector<std::tuple<std::shared_ptr<Variable>, bool>> varsToEstimate,
    std::vector<std::tuple<std::shared_ptr<Variable>, bool>> evidence, unsigned int numIterations) {

    // Chrono C("gibbsInference");
    // TODO : Gibbs evaluation

    return 1;
}

//************************************************/
//************** PRIVATE FUNCTIONS ***************/
//************************************************/
