#pragma once
#include "Rule.h"
#include "Vehicle.h"
#include "IViolationExpert.h"
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

class ControlZone {
private:
    string zoneId, zoneName;
    vector<Rule*> rules;
    IViolationExpert* expert;  // теперь интерфейс
    bool deleteViolations;

public:
    // Конструктор с экспертом (через интерфейс)
    ControlZone(const string& id, const string& name, IViolationExpert* exp)
        : zoneId(id), zoneName(name), expert(exp), deleteViolations(false) {}

    // Конструктор без эксперта
    ControlZone(const string& id, const string& name)
        : zoneId(id), zoneName(name), expert(nullptr), deleteViolations(true) {}

    void addRule(Rule* r) { rules.push_back(r); }

    vector<Violation*> check(Vehicle* v) {
        vector<Violation*> res;
        for (auto r : rules) {
            auto vio = r->check(v);
            if (vio) {
                res.push_back(vio);
                if (expert) expert->addViolation(vio);
            }
        }
        return res;
    }

    void printCheck(Vehicle* v) {
        cout << "\n[Зона " << zoneName << "]\n  ТС: " << v->id
            << ", скорость=" << v->speed << ", полоса=" << v->lane
            << ", тип=" << v->type << endl;

        auto violations = check(v);
        if (violations.empty()) {
            cout << "  -> Нарушений нет" << endl;
        }
        else {
            for (auto vio : violations) {
                cout << "  - " << vio->getDescription() << endl;
                if (deleteViolations) delete vio;
            }
        }
    }

    string getName() const { return zoneName; }
    string getId() const { return zoneId; }
};