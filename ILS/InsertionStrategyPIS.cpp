#include "InsertionStrategyPIS.h"

void InsertionStrategyPIS::execute(Params *params, Solution *s, std::vector<int> *cl,
                                   InsertionCriterion *insertionCriterion) {
    while (!cl->empty()) { // todo && (at least one customer k ∈ CL can be added to s)

        double minCost = std::numeric_limits<int>::max();
        int bestRouteId = std::numeric_limits<int>::max();
        InsertionCriterionResult bestResult;
        for (int routeId = 0; routeId < params->nbVehicles; routeId++) {
            InsertionCriterionResult *result = insertionCriterion->evaluateMinInsertionCost(
                    s->getRoute(routeId), cl, &(params->distanceMatrix));
            if (result->cost < minCost) {
                bestResult = *result;
                bestRouteId = routeId;
            }
        }
        s->addClient(bestRouteId, bestResult.clientId, bestResult.pos);
        cl->erase(std::remove(cl->begin(), cl->end(), bestResult.clientId), cl->end());
    }
}
