#pragma once
#include "ClonableViolation.h"
#include <string>
using namespace std;

class SpeedViolationWithClone : public ClonableViolation {
private:
    int speed;
    int limit;

public:
    SpeedViolationWithClone(Vehicle* v, int s, int l)
        : ClonableViolation(v), speed(s), limit(l) {}

    //  онструктор копировани€ дл€ клонировани€
    SpeedViolationWithClone(const SpeedViolationWithClone& other)
        : ClonableViolation(other.vehicle), speed(other.speed), limit(other.limit) {}

    SpeedViolationWithClone* clone() override {
        return new SpeedViolationWithClone(*this);
    }

    string getDescription() override {
        return "Speed: " + to_string(speed) + " > " + to_string(limit);
    }

    int getSpeed() const { return speed; }
    int getLimit() const { return limit; }
};