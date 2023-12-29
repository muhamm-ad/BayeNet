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

    // 1. Create a list of factors
    std::vector<Factor> factors;

    // 2. Create factors for each variable in the network
    for (const auto &var : variables) {
        factors.push_back(createFactor(var, evidence));
    }

    // 3. Order the variables using the Min-Fill heuristic
    auto orderedVariables = sortVariables(variables);

    // 4. Perform variable elimination
    for (int i = orderedVariables.size() - 1; i >= 0; --i) {
        const auto &V = orderedVariables[i];
        // If V is a hidden variable, sum out V from factors
        if (isHidden(V, varsToEstimate, evidence))
            factors = eliminateVar(V, factors);
    }

    // 5. Compute the pointwise product of the remaining factors
    auto [jointProbability, product] = multiplyFactors(factors, varsToEstimate, evidence);

    // 6. Normalize the result
    return normalize(jointProbability, product, evidence);
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

Factor BayesianNetwork::createFactor(
    const std::shared_ptr<Variable> &var,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &evidence) {
    std::vector<std::shared_ptr<Variable>> vars = {var};
    std::vector<bool> values;
    // Include the parent variables of var
    for (const auto &parent : var->getParents())
        vars.push_back(parent);

    Factor factor(vars);
    // Generate all possible combinations of parent values
    size_t numCombinations = pow(2, vars.size());
    for (size_t i = 0; i < numCombinations; ++i) {
        values.clear();
        for (size_t j = 0; j < vars.size(); ++j) {
            bool value = (i >> j) & 1;
            values.push_back(value);
        }
        // Check if the current combination matches the evidence
        bool consistentWithEvidence = true;
        for (const auto &[eVar, eVal] : evidence) {
            auto it = std::find(vars.begin(), vars.end(), eVar);
            if (it != vars.end()) {
                size_t index = std::distance(vars.begin(), it);
                if (values[index] != eVal) {
                    consistentWithEvidence = false;
                    break;
                }
            }
        }
        // If consistent with evidence, set the probability in the factor
        if (consistentWithEvidence) {
            std::vector<bool> parentValues(values.begin() + 1, values.end());
            double prob =
                (values[0] == 1) ? var->getProb(parentValues) : 1 - var->getProb(parentValues);
            factor.setProbability(values, prob);
        } else {
            factor.setProbability(values, 0.0);
        }
    }

    return factor;
}

std::vector<std::shared_ptr<Variable>>
BayesianNetwork::sortVariables(const std::vector<std::shared_ptr<Variable>> &variables) {
    std::vector<std::shared_ptr<Variable>> orderedVariables;
    std::set<std::shared_ptr<Variable>> remainingVariables(variables.begin(), variables.end());
    // Continue until all variables have been ordered
    while (!remainingVariables.empty()) {
        int minFill = std::numeric_limits<int>::max();
        std::shared_ptr<Variable> minFillVar;
        // Iterate over the remaining variables
        for (const auto &var : remainingVariables) {
            int fill = 0;
            // Compare all pairs of parents for the current variable
            for (const auto &parent1 : var->getParents()) {
                for (const auto &parent2 : var->getParents()) {
                    // If the parents are not equal and parent2 is not a parent of parent1,
                    // increment the fill counter
                    if (parent1 != parent2 &&
                        std::find(parent1->getParents().begin(), parent1->getParents().end(),
                                  parent2) == parent1->getParents().end()) {
                        ++fill;
                    }
                }
            }
            fill /= 2;
            // If the current fill value is less than the minimum found so far,
            // update the minimum and store the current variable
            if (fill < minFill) {
                minFill = fill;
                minFillVar = var;
            }
        }
        // Add the variable with the minimum fill value to the ordered list
        orderedVariables.push_back(minFillVar);
        // Remove the variable from the set of remaining variables
        remainingVariables.erase(minFillVar);
    }

    return orderedVariables;
}

bool BayesianNetwork::isHidden(
    const std::shared_ptr<Variable> &var,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &varsToEstimate,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &evidence) {
    // Iterate over the variables to be estimated
    for (const auto &[v, _] : varsToEstimate) {
        // If the current variable is in the list of variables to be estimated, it is not hidden
        if (v == var)
            return false;
    }
    // Iterate over the evidence variables
    for (const auto &[v, _] : evidence) {
        // If the current variable is in the list of evidence variables, it is not hidden
        if (v == var)
            return false;
    }

    return true;
}

std::vector<Factor> BayesianNetwork::eliminateVar(const std::shared_ptr<Variable> &varToEliminate,
                                                  const std::vector<Factor> &factors) {
    std::vector<Factor> newFactors; // Factors without the variable to be eliminated
    Factor factorToEliminate({});   // The factor to eliminate
    bool found = false;
    // Search for the factor that includes the variable to be eliminated
    for (const auto &factor : factors) {
        auto vars = factor.getVariables();
        if (std::find(vars.begin(), vars.end(), varToEliminate) != vars.end()) {
            // If this is the first factor that includes the variable, keep it
            // If we've found such factor before, multiply it with the current factor
            if (!found) {
                factorToEliminate = factor;
                found = true;
            } else
                factorToEliminate = multiplyFactors(factorToEliminate, factor);
        } else {
            // If the factor doesn't include the variable, add it to the new factors
            newFactors.push_back(factor);
        }
    }

    if (found) { // If we've found at least one factor that includes the variable
        std::vector<std::shared_ptr<Variable>> newVars;
        // Get all variables from the factor except the variable to be eliminated
        for (const auto &var : factorToEliminate.getVariables()) {
            if (var != varToEliminate)
                newVars.push_back(var);
        }
        // Create a new factor with these variables
        Factor newFactor(newVars);
        // Compute the sum of probabilities for all combinations of values of the new variables
        size_t numCombinations = pow(2, newVars.size());
        for (size_t i = 0; i < numCombinations; ++i) {
            std::vector<bool> values;
            for (size_t j = 0; j < newVars.size(); ++j) {
                bool value = (i >> j) & 1;
                values.push_back(value);
            }
            double sum = 0.0;
            // Calculate the sum of probabilities for both values of the eliminated variable
            for (bool value : {false, true}) {
                std::vector<bool> extendedValues = {value};
                extendedValues.insert(extendedValues.end(), values.begin(), values.end());
                sum += factorToEliminate.getProbability(extendedValues);
            }
            // Set the sum as the probability of the current combination of values
            newFactor.setProbability(values, sum);
        }
        newFactors.push_back(newFactor); // Add the new factor to the new factors
    }
    // Return the new factors
    return newFactors;
}

Factor BayesianNetwork::multiplyFactors(const Factor &f1, const Factor &f2) {
    // Start by creating a new vector containing the variables of both factors
    std::vector<std::shared_ptr<Variable>> allVars = f1.getVariables();
    for (const auto &var : f2.getVariables()) {
        // If a variable from the second factor is not in the combined variable vector, add it
        if (std::find(allVars.begin(), allVars.end(), var) == allVars.end()) {
            allVars.push_back(var);
        }
    }
    // Create a new factor (product) that will represent the product of the two factors
    Factor product(allVars);
    // The number of possible value combinations equals 2 to the power of the number of variables
    size_t numCombinations = pow(2, allVars.size());
    // Iterate over each possible value combination
    for (size_t i = 0; i < numCombinations; ++i) {
        std::vector<bool> values;
        for (size_t j = 0; j < allVars.size(); ++j) {
            // Compute the jth value of the ith combination using bitwise shifting and masking
            bool value = (i >> j) & 1;
            values.push_back(value);
        }
        // Construct the value vectors for the two factors
        std::vector<bool> valuesF1, valuesF2;
        for (const auto &var : f1.getVariables()) {
            // Find the position of the variable in the combined variable vector
            size_t pos =
                std::distance(allVars.begin(), std::find(allVars.begin(), allVars.end(), var));
            valuesF1.push_back(values[pos]);
        }
        for (const auto &var : f2.getVariables()) {
            size_t pos =
                std::distance(allVars.begin(), std::find(allVars.begin(), allVars.end(), var));
            valuesF2.push_back(values[pos]);
        }

        // The probability of a combination in the product is the product of the probabilities in
        // the factors
        double prob = f1.getProbability(valuesF1) * f2.getProbability(valuesF2);
        product.setProbability(values, prob);
    }
    // Return the resulting factor
    return product;
}

std::tuple<double, Factor> BayesianNetwork::multiplyFactors(
    const std::vector<Factor> &factors,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &varsToEstimate,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &evidence) {
    // If there are no factors, return a tuple containing 1.0 and an empty Factor object
    if (factors.empty()) {
        return {1.0, Factor({})};
    }
    // Begin with the first factor
    Factor product = factors[0];
    // Multiply the rest of the factors one by one
    for (size_t i = 1; i < factors.size(); ++i) {
        product = multiplyFactors(product, factors[i]);
    }
    // Calculate the number of variable value combinations
    size_t numCombinations = pow(2, product.getVariables().size());
    double result = 0.0;
    // Iterate through each possible combination
    for (size_t i = 0; i < numCombinations; ++i) {
        std::vector<bool> values;
        for (size_t j = 0; j < product.getVariables().size(); ++j) {
            // Compute the jth value of the ith combination using bitwise shifting and masking
            bool value = (i >> j) & 1;
            values.push_back(value);
        }
        // Assume that the combination matches the evidence and the variables to estimate
        bool match = true;
        // For each piece of evidence, check if the variable is present in the product factor
        for (const auto &[eVar, eVal] : evidence) {
            if (product.hasVariable(eVar)) {
                auto vs = product.getVariables();
                size_t index = std::distance(vs.begin(), std::find(vs.begin(), vs.end(), eVar));
                // If the value does not match the evidence, the combination does not match
                if (values[index] != eVal) {
                    match = false;
                    break;
                }
            }
        }
        // For each variable to estimate, check if the variable is present in the product factor
        for (const auto &[vVar, vVal] : varsToEstimate) {
            if (product.hasVariable(vVar)) {
                auto vs = product.getVariables();
                size_t index = std::distance(vs.begin(), std::find(vs.begin(), vs.end(), vVar));
                // If the value does not match the estimated value, the combination does not match
                if (values[index] != vVal) {
                    match = false;
                    break;
                }
            }
        }
        // If the combination matches both the evidence and the variables to estimate, update the
        // result
        if (match) {
            result = product.getProbability(values);
            break;
        }
    }
    // Return the result and the product factor
    return {result, product};
}

double BayesianNetwork::normalize(
    double jointProbability, Factor &product,
    const std::vector<std::tuple<std::shared_ptr<Variable>, bool>> &evidence) {
    // Calculate the total number of possible combinations of variable values
    size_t numCombinations = pow(2, product.getVariables().size());
    double sumObservedProbabilities = 0.0;
    // Iterate through each combination
    for (size_t i = 0; i < numCombinations; ++i) {
        std::vector<bool> values;
        for (size_t j = 0; j < product.getVariables().size(); ++j) {
            // Compute the jth value of the ith combination using bitwise shifting and masking
            bool value = (i >> j) & 1;
            values.push_back(value);
        }
        // Assume that the combination matches the evidence
        bool match = true;
        // For each piece of evidence, check if the variable is present in the product factor
        for (const auto &[eVar, eVal] : evidence) {
            if (product.hasVariable(eVar)) {
                auto vs = product.getVariables();
                size_t index = std::distance(vs.begin(), std::find(vs.begin(), vs.end(), eVar));
                // If the value does not match the evidence, the combination does not match
                if (values[index] != eVal) {
                    match = false;
                    break;
                }
            }
        }
        // If the combination matches the evidence, add its probability to the sum of observed
        // probabilities
        if (match)
            sumObservedProbabilities += product.getProbability(values);
    }
    // Normalize the joint probability by dividing it by the sum of observed probabilities
    return jointProbability / sumObservedProbabilities;
}
