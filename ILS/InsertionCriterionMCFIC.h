#ifndef ILS_INSERTIONCRITERIONMCFIC_H
#define ILS_INSERTIONCRITERIONMCFIC_H

#include "InsertionCriterion.h"
#include <random>

class InsertionCriterionMCFIC : public InsertionCriterion {
private:
    float gama;
    float randomDiscreteInterval(float from, float to, float interval);

public:
    InsertionCriterionResult* evaluateMinInsertionCost(std::vector<int> *routeSequence, std::vector<int> *cl,
                                  std::vector<std::vector<double>> *distanceMatrix) override;

    InsertionCriterionMCFIC();
};


#endif //ILS_INSERTIONCRITERIONMCFIC_H
