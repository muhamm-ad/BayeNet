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

    // Initialize count map to keep track of variables to estimate
    std::map<std::shared_ptr<Variable>, int> count;
    for (const auto &var : varsToEstimate)
        count[std::get<0>(var)] = 0;

    // Separate variables that are not part of the evidence
    std::set<std::shared_ptr<Variable>> nonEvidenceVars;
    for (const auto &variable : variables) {
        if (!isVarEvidence(variable, evidence))
            nonEvidenceVars.insert(variable);
    }

    // Initialize the current state
    std::map<std::shared_ptr<Variable>, bool> currentState;
    for (const auto &var : nonEvidenceVars)
        currentState[var] = var->sample();
    for (const auto &ev : evidence)
        currentState[std::get<0>(ev)] = std::get<1>(ev);

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nonEvidenceVars.size() - 1);

    // Main loop for Gibbs sampling
    for (unsigned int i = 0; i < numIterations; ++i) {
        for (const auto &var : nonEvidenceVars) {
            std::vector<bool> markovBlanket;
            for (const auto &parent : var->getParents()) {
                markovBlanket.push_back(currentState[parent]);
            }
            for (const auto &child : var->getChildren()) {
                for (const auto &parent : child->getParents()) {
                    if (parent != var)
                        markovBlanket.push_back(currentState[parent]);
                }
            }
            // Sample new state for current variable
            currentState[var] = var->sample(markovBlanket);
        }
        // Check if all conditions for estimation are met
        bool allConditionsMet = true;
        for (const auto &var : varsToEstimate) {
            if (currentState[std::get<0>(var)] != std::get<1>(var)) {
                allConditionsMet = false;
                break;
            }
        }

        // Update count if all conditions are met
        if (allConditionsMet) {
            for (const auto &var : varsToEstimate)
                count[std::get<0>(var)]++;
        }
    }
    // Compute total count
    double totalCount = 0;
    for (const auto &c : count) {
        totalCount += c.second;
    }
    // Return normalized count
    return totalCount / numIterations;
}

//************************************************/
//************** PRIVATE FUNCTIONS ***************/
//************************************************/

bool BayesianNetwork::isVarEvidence(
    const std::shared_ptr<Variable> &var,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> evidence) {
    for (const auto &ev : evidence) {
        if (var == std::get<0>(ev)) {
            return true;
        }
    }
    return false;
}