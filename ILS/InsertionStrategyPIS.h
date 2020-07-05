#ifndef ILS_INSERTIONSTRATEGYPIS_H
#define ILS_INSERTIONSTRATEGYPIS_H

#include "InsertionStrategy.h"

class InsertionStrategyPIS : public InsertionStrategy {
public:
    void execute(Params *params, Solution *s, std::vector<int> *cl, InsertionCriterion *insertionCriterion) override;
};


#endif //ILS_INSERTIONSTRATEGYPIS_H
