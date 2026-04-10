#pragma once
#include "Rule.h"
#include "Violation.h"
#include <string>
using namespace std;

class BusLaneViolation : public Violation {
private:
    int laneId;

public:
    BusLaneViolation(Vehicle* v, int lane) : Violation(v), laneId(lane) {}

    string getDescription() override {
        return "Bus lane violation on lane " + to_string(laneId);
    }
};

class BusLaneRule : public Rule {
private:
    int busLaneId;

public:
    BusLaneRule(int laneId) : busLaneId(laneId) {}

    Violation* check(Vehicle* v) override {
        if (v->lane == busLaneId && v->getType() != "Bus") {
            return new BusLaneViolation(v, busLaneId);
        }
        return nullptr;
    }
};