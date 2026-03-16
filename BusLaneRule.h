#pragma once
#include "Rule.h"
#include "BusLaneViolation.h"
class BusLaneRule : public Rule {
public:
    int busLane;
    BusLaneRule(int l) : busLane(l) {}
    Violation* check(Vehicle* v) override {
        if (v->lane == busLane && v->getType() != "Bus")
            return new BusLaneViolation(v);
        return nullptr;
    }
};