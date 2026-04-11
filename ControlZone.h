#pragma once
#include <vector>
#include <iostream>    
#include "Rule.h"
#include "Violation.h"
#include "Vehicle.h"

using namespace std;

class ControlZone {
protected:
    vector<Rule*> rules;
    int zoneId;
    bool ownsRules;

public:
    // Обычный конструктор
    ControlZone(int id = 0, bool ownRules = true)
        : zoneId(id), ownsRules(ownRules) {}

    // КОНСТРУКТОР КОПИРОВАНИЯ (для прототипа)
    ControlZone(const ControlZone& other)
        : zoneId(other.zoneId + 1000), ownsRules(false) {
        // Копируем указатели на правила (мелкое копирование)
        for (auto r : other.rules) {
            rules.push_back(r);
        }
        cout << "[ControlZone] Клонирование: создана зона ID=" << zoneId
            << " из оригинала ID=" << other.zoneId << endl;
    }

    // Деструктор
    virtual ~ControlZone() {
        if (ownsRules) {
            for (auto r : rules) {
                delete r;
            }
        }
        rules.clear();
    }

    // Метод клонирования (Prototype)
    ControlZone* clone() {
        return new ControlZone(*this);
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