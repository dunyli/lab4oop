#pragma once
#include "Camera.h"
#include "Vehicle.h"
#include "ControlZone.h"
#include "RuleFactory.h"
#include <vector>
#include <iostream>
using namespace std;

class TrafficMonitorFacade {
private:
    Camera* camera;
    RuleFactory ruleFactory;
    vector<ControlZone*> zones;

public:
    TrafficMonitorFacade(Camera* cam) : camera(cam) {
        cout << "[Фасад] Система видеонаблюдения инициализирована" << endl;
    }

    ~TrafficMonitorFacade() {
        for (auto zone : zones) delete zone;
    }

    // ===== СОЗДАНИЕ ЗОН (без эксперта) =====

    void createSpeedZone(const string& zoneId, const string& zoneName, int speedLimit) {
        // Используем конструктор без эксперта
        ControlZone* zone = new ControlZone(zoneId, zoneName);
        zone->addRule(ruleFactory.getSpeedRule(speedLimit, zoneName));
        zones.push_back(zone);
        cout << "[Фасад] Создана зона '" << zoneName << "' с лимитом " << speedLimit << " км/ч" << endl;
    }

    void createBusLaneZone(const string& zoneId, const string& zoneName, int laneNumber, int speedLimit) {
        // Используем конструктор без эксперта
        ControlZone* zone = new ControlZone(zoneId, zoneName);
        zone->addRule(ruleFactory.getSpeedRule(speedLimit, zoneName));
        zone->addRule(ruleFactory.getBusLaneRule(laneNumber, zoneName));
        zones.push_back(zone);
        cout << "[Фасад] Создана зона '" << zoneName << "' с лимитом " << speedLimit
            << " км/ч и выделенной полосой " << laneNumber << endl;
    }

    // ===== ПРОВЕРКА АВТОМОБИЛЯ =====

    void checkVehicleInAllZones(Vehicle* v) {
        cout << "\n[Фасад] Проверка ТС " << v->id << " во всех зонах:" << endl;
        for (auto zone : zones) {
            zone->printCheck(v);
        }
    }

    void checkVehicleInZone(Vehicle* v, const string& zoneName) {
        for (auto zone : zones) {
            if (zone->getName() == zoneName) {
                cout << "\n[Фасад] Проверка ТС " << v->id << " в зоне '" << zoneName << "':" << endl;
                zone->printCheck(v);
                return;
            }
        }
        cout << "[Фасад] Зона '" << zoneName << "' не найдена" << endl;
    }

    void captureAndCheck(const string& vehicleId, int speed, int lane, const string& type) {
        cout << "\n[Фасад] Полная проверка с захватом кадра:" << endl;

        Frame* frame = camera->getFrame();
        cout << "  Кадр получен, timestamp=" << frame->timestamp << endl;

        Vehicle v(vehicleId, type);
        v.speed = speed;
        v.lane = lane;

        checkVehicleInAllZones(&v);

        delete frame;
    }

    void printAllZones() {
        cout << "\n[Фасад] Список всех зон контроля:" << endl;
        for (auto zone : zones) {
            cout << "  - " << zone->getName() << endl;
        }
    }

    // Быстрые методы
    bool isSpeedViolation(int speed, int limit) {
        return speed > limit;
    }

    int calculateFine(int speed, int limit) {
        int excess = speed - limit;
        if (excess <= 0) return 0;
        if (excess <= 20) return 500;
        if (excess <= 40) return 1500;
        return 3000;
    }

    int getZoneCount() const { return zones.size(); }
};