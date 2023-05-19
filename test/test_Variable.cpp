#include <gtest/gtest.h>
#include "../include/Variable.hpp"
#include <memory>

class VariableTest : public ::testing::Test {
protected:
    std::shared_ptr<Variable> variable;

    void SetUp() override {
        variable = std::make_shared<Variable>("Variable1", std::vector<std::shared_ptr<Variable>>());
    }
};

// Testing GetName method
TEST_F(VariableTest, GetNameTest) {
    EXPECT_EQ(variable->getName(), "Variable1");
}

// Testing SetName method
TEST_F(VariableTest, SetNameTest) {
    variable->setName("Variable2");
    EXPECT_EQ(variable->getName(), "Variable2");
}

// Testing getVal and setVal methods
TEST_F(VariableTest, ValTest) {
    variable->setVal(true);
    EXPECT_EQ(variable->getVal(), true);
    variable->setVal(false);
    EXPECT_EQ(variable->getVal(), false);
}

// Testing getParents method
TEST_F(VariableTest, GetParentsTest) {
    auto parents = variable->getParents();
    EXPECT_EQ(parents.size(), 0);
}

// Testing getChildren method
TEST_F(VariableTest, GetChildrenTest) {
    auto children = variable->getChildren();
    EXPECT_EQ(children.size(), 0);
}

// Testing getProb and setProb methods
TEST_F(VariableTest, ProbTest) {
    std::vector<bool> parentValues = {false, true, true}; 
    variable->setProb(parentValues, 0.7);  // Setting the probability when parent values are {false, true, true} to 0.7
    EXPECT_NEAR(variable->getProb(parentValues), 0.7, 0.001);  // Checking the set probability. The third parameter to EXPECT_NEAR is the absolute error tolerance.
}

// Testing sample method
TEST_F(VariableTest, SampleTest) {
    // Set up the probability table
    std::vector<bool> parentValues = {false, true, true};
    variable->setProb(parentValues, 0.7); // Assuming that the state {false, true, true} occurs with a probability of 0.7
    variable->setProb(parentValues, 0.3); // Assuming that any other state occurs with a probability of 0.3

    // Count how many times the variable becomes true over a large number of samples
    int trueCount = 0;
    for (int i = 0; i < 10000; ++i) {
        if (variable->sample()) {
            trueCount++;
        }
    }

    // Expect that the variable is true about 70% of the time
    EXPECT_NEAR(trueCount / 10000.0, 0.7, 0.05);  // The third parameter to EXPECT_NEAR is the absolute error tolerance.
}

// ...
