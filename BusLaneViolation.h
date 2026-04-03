#pragma once
#include "Violation.h"
using namespace std;

class BusLaneViolation : public Violation {
private:
    int laneNumber;
public:
    BusLaneViolation(Vehicle* v, int lane, const string& zone, long time)
        : Violation(v, zone, time), laneNumber(lane) {}

    string getDescription() override {
        return "Движение по выделенной полосе " + to_string(laneNumber);
    }

    int getFine() override {
        return 2500;
    }
};