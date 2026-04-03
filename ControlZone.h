#pragma once
#include "Rule.h"
#include "Vehicle.h"
#include "ViolationExpert.h"
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

class ControlZone {
private:
    string zoneId, zoneName;
    vector<Rule*> rules;
    ViolationExpert* expert;
    bool deleteViolations;  // флаг, нужно ли удалять нарушения

public:
    // Конструктор с экспертом (нарушения НЕ удаляем, эксперт сам удалит)
    ControlZone(const string& id, const string& name, ViolationExpert* exp)
        : zoneId(id), zoneName(name), expert(exp), deleteViolations(false) {}

    // Конструктор без эксперта (нарушения удаляем)
    ControlZone(const string& id, const string& name)
        : zoneId(id), zoneName(name), expert(nullptr), deleteViolations(true) {}

    void addRule(Rule* r) { rules.push_back(r); }

    vector<Violation*> check(Vehicle* v) {
        vector<Violation*> res;
        for (auto r : rules) {
            auto vio = r->check(v);
            if (vio) {
                res.push_back(vio);
                if (expert) {
                    expert->addViolation(vio);  // эксперт теперь владеет нарушением
                }
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
                cout << "  -> " << vio->getDescription() << endl;
                if (expert && expert->calculateFineWithHistory(vio) > 0) {
                    cout << "     Штраф с учётом истории: "
                        << expert->calculateFineWithHistory(vio) << " руб." << endl;
                }
                // НЕ удаляем vio, если есть эксперт (эксперт сам удалит)
                if (deleteViolations) {
                    delete vio;
                }
            }
        }
    }

    string getName() const { return zoneName; }
    string getId() const { return zoneId; }
};