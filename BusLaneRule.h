#pragma once
#include "Rule.h"
#include "BusLaneViolation.h"
#include <ctime>
using namespace std;

class BusLaneRule : public Rule {
private:
    int busLaneNumber;
    string zoneName;  // добавляем имя зоны
public:
    BusLaneRule(int laneNum, const string& zone) : busLaneNumber(laneNum), zoneName(zone) {}

    Violation* check(Vehicle* v) override {
        if (v->lane == busLaneNumber && v->type != "Bus" && v->type != "Taxi") {
            return new BusLaneViolation(v, busLaneNumber, zoneName, time(nullptr));
        }
        return nullptr;
    }

    int getLaneNumber() const { return busLaneNumber; }
};