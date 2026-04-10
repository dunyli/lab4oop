#pragma once
#include "Rule.h"
#include "Violation.h"
#include <string>
#include <cstdlib>
using namespace std;

class LineCrossViolation : public Violation {
public:
    LineCrossViolation(Vehicle* v) : Violation(v) {}
    string getDescription() override { return "Line cross violation"; }
};

class LineCrossRule : public Rule {
public:
    Violation* check(Vehicle* v) override {
        if (rand() % 100 < 20) {
            return new LineCrossViolation(v);
        }
        return nullptr;
    }
};