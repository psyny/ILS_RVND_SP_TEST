#ifndef ILS_INSERTIONCRITERIONNIFC_H
#define ILS_INSERTIONCRITERIONNIFC_H

#include "InsertionCriterion.h"

class InsertionCriterionNIFC : public InsertionCriterion {
public:
    InsertionCriterionResult* evaluateMinInsertionCost(std::vector<int> *routeSequence, std::vector<int> *cl,
                                  std::vector<std::vector<double>> *distanceMatrix) override;
};


#endif //ILS_INSERTIONCRITERIONNIFC_H
