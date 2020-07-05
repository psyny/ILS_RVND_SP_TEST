#include "InsertionCriterionMCFIC.h"
#include <limits>

float InsertionCriterionMCFIC::randomDiscreteInterval(float from, float to, float interval) {
    int size = ((to - from) / interval ) + 1;
    std::vector<float> numbers = std::vector<float>(size);

    for (int i = 0; i < size; i++) {
        numbers[i] = from + (i * interval);
    }

    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> choose(0, size - 1);

    return numbers[choose(engine)];
}


InsertionCriterionMCFIC::InsertionCriterionMCFIC() {
    // As defined  in  Subramanian et al. (2010);
    gama = randomDiscreteInterval(0.00, 1.70, 0.05);
}

InsertionCriterionResult *
InsertionCriterionMCFIC::evaluateMinInsertionCost(std::vector<int> *routeSequence, std::vector<int> *cl,
                                                  std::vector<std::vector<double>> *distanceMatrix) {
    int minGlobalPos = std::numeric_limits<int>::max();
    double minGlobalCost = std::numeric_limits<double>::max();
    int minGlobalClientId = std::numeric_limits<int>::max();
    for (int i = 0; i < cl->size(); i++) {
        int outClientId = (*cl)[i];
        int minPos = std::numeric_limits<int>::max();
        double minCost = std::numeric_limits<double>::max();
        double depotClientCost = (*distanceMatrix)[0][outClientId];
        double clientDepotCost = (*distanceMatrix)[outClientId][0];
        double gamaCost = gama * (depotClientCost + clientDepotCost);

        // FixMe paper doesn't talk about how to proceed with 1 element
        if (routeSequence->size() == 1) {
            int inClientId = (*routeSequence)[0];
            double cost = (*distanceMatrix)[inClientId][outClientId];      // g(k)

            if (cost < minCost) {
                minCost = cost;
                minPos = 1;
            }
        }

        // FixMe paper doesn't talk about add at end
        for (int j = 1; j < routeSequence->size(); j++) {
            int prevClientId = (*routeSequence)[j-1];
            int nextClientId = (*routeSequence)[j];

            double prevCost = (*distanceMatrix)[prevClientId][outClientId];
            double nextCost = (*distanceMatrix)[outClientId][nextClientId];
            double inCost = (*distanceMatrix)[prevClientId][nextClientId];

            double cost = (prevCost + nextCost - inCost) - gamaCost;      // g(k)

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
