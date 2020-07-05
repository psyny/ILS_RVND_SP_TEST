#ifndef ILS_INSERTIONSTRATEGYSIS_H
#define ILS_INSERTIONSTRATEGYSIS_H

#include "InsertionStrategy.h"

class InsertionStrategySIS : public InsertionStrategy {
public:
    void execute(Params *params, Solution *s, std::vector<int> *cl, InsertionCriterion *insertionCriterion) override;
};


#endif //ILS_INSERTIONSTRATEGYSIS_H
