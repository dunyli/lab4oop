#pragma once
#include <string>
#include "Vehicle.h"
class Violation {
public:
    Vehicle* vehicle;
    Violation(Vehicle* v) : vehicle(v) {}
    // Конструктор по умолчанию (нужен для ViolationDecorator)
    Violation() : vehicle(nullptr) {}
    virtual ~Violation() {}
    virtual std::string getDescription() { return "Violation"; }
};