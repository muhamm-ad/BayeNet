#include "../include/BayesianNetwork.hpp"
#include "gtest/gtest.h"

// Define the number of runs for each test
const int NUM_RUNS = 10;

class BayesianNetworkTest : public ::testing::Test {
  protected:
    void SetUp() override { srand(time(NULL)); }

    // Define Bayesian networks used in tests

    BayesianNetwork firstNetwork() {
        BayesianNetwork BN;
        auto weekEnd = BN.addBooleanVar("weekEnd", 0.3);
        auto rain = BN.addBooleanVar("rain", 0.1);

        auto traffic = BN.addBooleanVar("traffic", weekEnd, rain);
        traffic->setProb(false, false, 0.5);
        traffic->setProb(false, true, 0.9);
        traffic->setProb(true, false, 0.01);
        traffic->setProb(true, true, 0.1);

        auto umbrella = BN.addBooleanVar("umbrella", rain);
        umbrella->setProb(true, 0.9);
        umbrella->setProb(false, 0.01);

        return BN;
    }

    BayesianNetwork secondNetwork() {
        BayesianNetwork BN;
        auto earthquake = BN.addBooleanVar("earthquake", 0.001);
        auto burglary = BN.addBooleanVar("burglary", 0.002);

        auto alarm = BN.addBooleanVar("alarm", burglary, earthquake);
        alarm->setProb(true, true, 0.95);
        alarm->setProb(true, false, 0.94);
        alarm->setProb(false, true, 0.29);
        alarm->setProb(false, false, 0.001);

        auto johnCalls = BN.addBooleanVar("johnCalls", alarm);
        johnCalls->setProb(true, 0.9);
        johnCalls->setProb(false, 0.05);

        auto maryCalls = BN.addBooleanVar("maryCalls", alarm);
        maryCalls->setProb(true, 0.7);
        maryCalls->setProb(false, 0.01);

        return BN;
    }
};

// The tests verify the performance of various inference methods
// by running them multiple times and averaging the results.

// Test the exactInference method with two scenarios
TEST_F(BayesianNetworkTest, exactInference) {
    {
        BayesianNetwork BN = firstNetwork();
        auto weekEnd = BN.getVar("weekEnd");
        auto rain = BN.getVar("rain");
        auto traffic = BN.getVar("traffic");
        auto umbrella = BN.getVar("umbrella");

        // Initialize a variable to accumulate the results
        double totalExactValue_1 = 0.0, totalExactValue_2 = 0.0;
        // Run the test NUM_RUNS times
        for (int i = 0; i < NUM_RUNS; ++i) {
            totalExactValue_1 += BN.exactInference({{traffic, true}}, {{umbrella, true}});
            totalExactValue_2 +=
                BN.exactInference({{traffic, true}, {weekEnd, false}}, {{umbrella, true}});
        }
        // Calculate the average result
        double avgExactValue_1 = totalExactValue_1 / NUM_RUNS;
        double avgExactValue_2 = totalExactValue_2 / NUM_RUNS;
        // Check that the average result is close to the expected value
        EXPECT_NEAR(avgExactValue_1, 0.632091, 0.001);
        EXPECT_NEAR(avgExactValue_2, 0.604545, 0.001);
    }
    {
        BayesianNetwork BN = secondNetwork();
        auto earthquake = BN.getVar("earthquake");
        auto burglary = BN.getVar("burglary");
        auto alarm = BN.getVar("alarm");
        auto johnCalls = BN.getVar("johnCalls");
        auto maryCalls = BN.getVar("maryCalls");

        // Initialize a variable to accumulate the results
        double totalExactValue_3 = 0.0, totalExactValue_4 = 0.0;
        // Run the test NUM_RUNS times
        for (int i = 0; i < NUM_RUNS; ++i) {
            totalExactValue_3 +=
                BN.exactInference({{burglary, true}}, {{johnCalls, true}, {maryCalls, false}});
            totalExactValue_4 += BN.exactInference({{burglary, false}, {alarm, true}},
                                                   {{johnCalls, true}, {maryCalls, true}});
        }
        // Calculate the average result
        double avgExactValue_3 = totalExactValue_3 / NUM_RUNS;
        double avgExactValue_4 = totalExactValue_4 / NUM_RUNS;
        // Check that the average result is close to the expected value
        EXPECT_NEAR(avgExactValue_3, 0.0102303, 0.001);
        EXPECT_NEAR(avgExactValue_4, 0.325053, 0.001);
    }
}

// Test the approximateInference method with two scenarios
TEST_F(BayesianNetworkTest, approximateInference) {
    {
        BayesianNetwork BN = firstNetwork();
        auto weekEnd = BN.getVar("weekEnd");
        auto rain = BN.getVar("rain");
        auto traffic = BN.getVar("traffic");
        auto umbrella = BN.getVar("umbrella");

        // Initialize a variable to accumulate the results
        double totalApproxValue_1 = 0.0, totalApproxValue_2 = 0.0;
        // Run the test NUM_RUNS times
        for (int i = 0; i < NUM_RUNS; ++i) {
            totalApproxValue_1 +=
                BN.approximateInference({{traffic, true}}, {{umbrella, true}}, 10000);
            totalApproxValue_2 += BN.approximateInference({{traffic, true}, {weekEnd, false}},
                                                          {{umbrella, true}}, 10000);
        }
        // Calculate the average result
        double avgApproxValue_1 = totalApproxValue_1 / NUM_RUNS;
        double avgApproxValue_2 = totalApproxValue_2 / NUM_RUNS;
        // Check that the average result is close to the expected value
        EXPECT_NEAR(avgApproxValue_1, 0.632091, 0.06);
        EXPECT_NEAR(avgApproxValue_2, 0.604545, 0.06);
    }
    {
        BayesianNetwork BN = secondNetwork();
        auto earthquake = BN.getVar("earthquake");
        auto burglary = BN.getVar("burglary");
        auto alarm = BN.getVar("alarm");
        auto johnCalls = BN.getVar("johnCalls");
        auto maryCalls = BN.getVar("maryCalls");

        // Initialize a variable to accumulate the results
        double totalApproxValue_3 = 0.0, totalApproxValue_4 = 0.0;
        // Run the test NUM_RUNS times
        for (int i = 0; i < NUM_RUNS; ++i) {
            totalApproxValue_3 += BN.approximateInference(
                {{burglary, true}}, {{johnCalls, true}, {maryCalls, false}}, 10000);
            totalApproxValue_4 += BN.approximateInference(
                {{burglary, false}, {alarm, true}}, {{johnCalls, true}, {maryCalls, true}}, 10000);
        }
        // Calculate the average result
        double avgApproxValue_3 = totalApproxValue_3 / NUM_RUNS;
        double avgApproxValue_4 = totalApproxValue_4 / NUM_RUNS;
        // Check that the average result is close to the expected value
        EXPECT_NEAR(avgApproxValue_3, 0.0102303, 0.004);
        EXPECT_NEAR(avgApproxValue_4, 0.325053, 0.01);
    }
}

// Test the gibbsInference method with two scenarios
TEST_F(BayesianNetworkTest, gibbsInference) {
    {
        BayesianNetwork BN = firstNetwork();
        auto weekEnd = BN.getVar("weekEnd");
        auto rain = BN.getVar("rain");
        auto traffic = BN.getVar("traffic");
        auto umbrella = BN.getVar("umbrella");

        // Initialize a variable to accumulate the results
        double totalGibbsValue_1 = 0.0, totalGibbsValue_2 = 0.0;
        // Run the test NUM_RUNS times
        for (int i = 0; i < NUM_RUNS; ++i) {
            totalGibbsValue_1 += BN.gibbsInference({{traffic, true}}, {{umbrella, true}}, 10000);
            totalGibbsValue_2 +=
                BN.gibbsInference({{traffic, true}, {weekEnd, false}}, {{umbrella, true}}, 10000);
        }
        // Calculate the average result
        double avgGibbsValue_1 = totalGibbsValue_1 / NUM_RUNS;
        double avgGibbsValue_2 = totalGibbsValue_2 / NUM_RUNS;
        // Check that the average result is close to the expected value
        EXPECT_NEAR(avgGibbsValue_1, 0.632091, 0.06);
        EXPECT_NEAR(avgGibbsValue_2, 0.604545, 0.06);
    }
    {
        BayesianNetwork BN = secondNetwork();
        auto earthquake = BN.getVar("earthquake");
        auto burglary = BN.getVar("burglary");
        auto alarm = BN.getVar("alarm");
        auto johnCalls = BN.getVar("johnCalls");
        auto maryCalls = BN.getVar("maryCalls");

        // Initialize a variable to accumulate the results
        double totalGibbsValue_3 = 0.0, totalGibbsValue_4 = 0.0;
        // Run the test NUM_RUNS times
        for (int i = 0; i < NUM_RUNS; ++i) {
            totalGibbsValue_3 += BN.gibbsInference({{burglary, true}},
                                                   {{johnCalls, true}, {maryCalls, false}}, 100000);
            totalGibbsValue_4 += BN.gibbsInference({{burglary, false}, {alarm, true}},
                                                   {{johnCalls, true}, {maryCalls, true}}, 100000);
        }
        // Calculate the average result
        double avgGibbsValue_3 = totalGibbsValue_3 / NUM_RUNS;
        double avgGibbsValue_4 = totalGibbsValue_4 / NUM_RUNS;
        // Check that the average result is close to the expected value
        EXPECT_NEAR(avgGibbsValue_3, 0.0102303, 0.01);
        EXPECT_NEAR(avgGibbsValue_4, 0.325053, 0.01);
    }
}
