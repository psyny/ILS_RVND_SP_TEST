#ifndef ILS_INSERTIONCRITERION_H
#define ILS_INSERTIONCRITERION_H

#include "vector"

struct InsertionCriterionResult {
    int pos;        // the position to insert after
    int clientId;   // the client id
    double cost;    // the client id
};

class InsertionCriterion {
public:
    virtual InsertionCriterionResult* evaluateMinInsertionCost(std::vector<int> *routeSequence, std::vector<int> *cl,
                                          std::vector<std::vector<double> > *distanceMatrix) = 0;
};


#endif //ILS_INSERTIONCRITERION_H
