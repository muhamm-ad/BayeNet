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

    // Initialize the counter of compatible samples
    unsigned int compatibleEvidence = 0;
    unsigned int compatibleInference = 1;

    // Loop to generate samples
    for (unsigned int i = 0; i < numIterations; ++i) {
        // Generate a sample for each variable in the Bayesian network
        std::map<std::shared_ptr<Variable>, bool> sample;
        for (const auto &var : variables) {
            std::map<std::shared_ptr<Variable>, bool> parent_sample;
            for (const auto &parent : var->getParents()) {
                parent_sample[parent] = sample[parent];
            }
            bool v = var->sample(parent_sample);
            sample[var] = v;
        }

        // If the sample is compatible, increment the counter
        if (isConsistent(sample, evidence)) {
            ++compatibleEvidence;
            // Count the compatible samples that correspond to the queries
            if (isConsistent(sample, varsToEstimate))
                ++compatibleInference;
        }
    }
    // Calculate the normalized probability by dividing the number of compatible samples by the
    // total number of iterations
    return static_cast<double>(compatibleInference) / static_cast<double>(compatibleEvidence);

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

bool BayesianNetwork::isConsistent(
    std::map<std::shared_ptr<Variable>, bool> &sample,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &observations) {
    // Check if the sample is consistent with the evidence
    for (const auto &[Var, Val] : observations) {
        if (sample[Var] != Val) {
            return false;
        }
    }
    return true;
}