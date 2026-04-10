#pragma once
#include "ClonableViolation.h"
#include <string>

using namespace std;

class BusLaneViolationWithClone : public ClonableViolation {
private:
    int laneId;           // номер выделенной полосы
    string vehicleType;   // тип ТС, которое нарушило

public:
    // Обычный конструктор
    BusLaneViolationWithClone(Vehicle* v, int lane, string vehType)
        : ClonableViolation(v), laneId(lane), vehicleType(vehType) {}

    // Конструктор копирования (для клонирования)
    BusLaneViolationWithClone(const BusLaneViolationWithClone& other)
        : ClonableViolation(other.vehicle),
        laneId(other.laneId),
        vehicleType(other.vehicleType) {}

    // Метод клонирования
    BusLaneViolationWithClone* clone() override {
        return new BusLaneViolationWithClone(*this);
    }

    string getDescription() override {
        return "Bus lane violation on lane " + to_string(laneId) +
            " (vehicle type: " + vehicleType + ")";
    }
};