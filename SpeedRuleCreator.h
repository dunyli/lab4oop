#pragma once
#include "RuleCreator.h"
#include "SpeedRule.h"

class SpeedRuleCreator : public RuleCreator {
private:
    int speedLimit;

public:
    SpeedRuleCreator(int limit = 60) : speedLimit(limit) {}

    Rule* createRule() override {
        return new SpeedRule(speedLimit);
    }
};