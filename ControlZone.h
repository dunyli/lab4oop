#pragma once
#include <vector>
#include "Rule.h"
#include "Violation.h"
#include "Vehicle.h"

using namespace std;

class ControlZone {
protected:
    vector<Rule*> rules;
    int zoneId;
    bool ownsRules;  // Флаг, указывающий, должна ли зона удалять правила

public:
    ControlZone(int id = 0, bool ownRules = true)
        : zoneId(id), ownsRules(ownRules) {}

    virtual ~ControlZone() {
        // Удаляем правила только если зона является их владельцем
        if (ownsRules) {
            for (auto r : rules) {
                delete r;
            }
        }
        rules.clear();  // Очищаем вектор без удаления
    }

    void addRule(Rule* r) {
        if (r != nullptr) {
            rules.push_back(r);
        }
    }

    void setOwnsRules(bool own) {
        ownsRules = own;
    }

    vector<Violation*> check(Vehicle* v) {
        vector<Violation*> result;
        for (auto r : rules) {
            if (r != nullptr) { 
                Violation* vio = r->check(v);
                if (vio != nullptr) {
                    result.push_back(vio);
                }
            }
        }
        return result;
    }

    int getZoneId() const { return zoneId; }
    int getRulesCount() const { return rules.size(); }
};