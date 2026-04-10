#pragma once
#include "Rule.h"
#include "Violation.h"
#include <string>
#include <cstdlib>
using namespace std;

class RedLightViolation : public Violation {
public:
    RedLightViolation(Vehicle* v) : Violation(v) {}
    string getDescription() override { return "Red light violation"; }
};

class TrafficLightRule : public Rule {
public:
    Violation* check(Vehicle* v) override {
        if (rand() % 100 < 25) {
            return new RedLightViolation(v);
        }
        return nullptr;
    }
};