#include "InsertionCriterionNIFC.h"

InsertionCriterionResult *
InsertionCriterionNIFC::evaluateMinInsertionCost(std::vector<int> *routeSequence, std::vector<int> *cl,
                                                 std::vector<std::vector<double>> *distanceMatrix) {
    int minGlobalPos = std::numeric_limits<int>::max();
    double minGlobalCost = std::numeric_limits<double>::max();
    int minGlobalClientId = std::numeric_limits<int>::max();
    for (auto const &outClientId: *cl) {
        int minPos = std::numeric_limits<int>::max();
        double minCost = std::numeric_limits<double>::max();

        // FixMe paper doesn't talk about how to proceed with 1 element
        if (routeSequence->size() == 1) {
            int inClientId = (*routeSequence)[0];
            double cost = (*distanceMatrix)[inClientId][outClientId];      // g(k)

            if (cost < minCost) {
                minCost = cost;
                minPos = 1;
            }
        }

        for (int j = 1; j < routeSequence->size(); j++) {
            int prevClientId = (*routeSequence)[j];
            double cost = (*distanceMatrix)[prevClientId][outClientId];      // g(k)

            if (cost < minCost) {
                minCost = cost;
                minPos = j;
            }
        }

        if (minCost < minGlobalCost) {
            minGlobalCost = minCost;
            minGlobalPos = minPos;
            minGlobalClientId = outClientId;
        }
    }

    auto *result = new InsertionCriterionResult();
    result->pos = minGlobalPos;
    result->clientId = minGlobalClientId;
    result->cost = minGlobalCost;
    return result;
}
