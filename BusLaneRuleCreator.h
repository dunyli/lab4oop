#pragma once
#include "RuleCreator.h"
#include "BusLaneRule.h"

class BusLaneRuleCreator : public RuleCreator {
private:
    int laneId;

public:
    BusLaneRuleCreator(int id = 1) : laneId(id) {}

    Rule* createRule() override {
        return new BusLaneRule(laneId);
    }
};