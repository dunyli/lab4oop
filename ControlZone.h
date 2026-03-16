#pragma once
#include <vector>
#include "Rule.h"
class ControlZone {
public:
    std::vector<Rule*> rules;
    ~ControlZone() { for (auto r : rules) delete r; }
    void addRule(Rule* r) { rules.push_back(r); }
    std::vector<Violation*> check(Vehicle* v) {
        std::vector<Violation*> res;
        for (auto r : rules) {
            auto vio = r->check(v);
            if (vio) res.push_back(vio);
        }
        return res;
    }
};