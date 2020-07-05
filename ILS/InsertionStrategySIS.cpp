#include "InsertionStrategySIS.h"
#include <algorithm>

void InsertionStrategySIS::execute(Params *params, Solution *s, std::vector<int> *cl,
                                   InsertionCriterion *insertionCriterion) {
    while (!cl->empty()) { // todo && (at least one customer k ∈ CL can be inserted into the solution)
        for (int routeId = 0; routeId < params->nbVehicles; routeId++) {
            if (cl->empty()) {
                break;
            }

            //todo if (at least one customer k ∈ CL can be inserted into the vehicle v′) then
            InsertionCriterionResult *result = insertionCriterion->evaluateMinInsertionCost(
                    s->getRoute(routeId), cl, &(params->distanceMatrix));

            s->addClient(routeId, result->clientId, result->pos);
            cl->erase(std::remove(cl->begin(), cl->end(), result->clientId), cl->end());
        }
    }
}
