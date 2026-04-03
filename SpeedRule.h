#pragma once
#include "Rule.h"
#include "SpeedViolation.h"
#include <ctime>
using namespace std;

class SpeedRule : public Rule {
private:
    int speedLimit;
    string zoneName;  // добавляем имя зоны
public:
    SpeedRule(int limit, const string& zone) : speedLimit(limit), zoneName(zone) {}

    Violation* check(Vehicle* v) override {
        if (v->speed > speedLimit) {
            return new SpeedViolation(v, v->speed, speedLimit, zoneName, time(nullptr));
        }
        return nullptr;
    }

    int getSpeedLimit() const { return speedLimit; }
};