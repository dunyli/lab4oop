#pragma once
#include "Violation.h"
#include "Vehicle.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

class ViolationExpert {
private:
    vector<Violation*> allViolations;
    map<string, vector<Violation*>> violationsByVehicle;
    map<string, vector<Violation*>> violationsByZone;
    map<int, vector<Violation*>> violationsByHour;

public:
    ~ViolationExpert() {
        // Эксперт удаляет все нарушения при своём разрушении
        for (auto v : allViolations) {
            delete v;
        }
    }

    void addViolation(Violation* v) {
        allViolations.push_back(v);
        violationsByVehicle[v->vehicle->id].push_back(v);
        violationsByZone[v->zoneName].push_back(v);

        time_t t = v->timestamp;
        tm lt;
        localtime_s(&lt, &t);
        violationsByHour[lt.tm_hour].push_back(v);

        cout << "  [Эксперт] Зафиксировано: " << v->getDescription() << endl;
    }

    int calculateFineWithHistory(Violation* v) {
        if (!v) return 0;

        int baseFine = v->getFine();
        int count = violationsByVehicle[v->vehicle->id].size();

        if (count >= 3) {
            cout << "  [Эксперт] 3-е нарушение! Штраф +50%" << endl;
            return baseFine * 1.5;
        }
        if (count >= 2) {
            cout << "  [Эксперт] Повторное нарушение! Штраф +20%" << endl;
            return baseFine * 1.2;
        }
        return baseFine;
    }

    int getTotalCount() { return allViolations.size(); }

    void printStats() {
        cout << "\n=== СТАТИСТИКА ОТ ЭКСПЕРТА ===" << endl;
        cout << "Всего нарушений: " << allViolations.size() << endl;

        cout << "\nПо зонам:" << endl;
        for (auto& p : violationsByZone) {
            cout << "  " << p.first << ": " << p.second.size() << endl;
        }

        cout << "\nТоп нарушителей:" << endl;
        vector<pair<string, int>> offenders;
        for (auto& p : violationsByVehicle) {
            offenders.push_back({ p.first, p.second.size() });
        }
        sort(offenders.begin(), offenders.end(),
            [](auto& a, auto& b) { return a.second > b.second; });

        for (int i = 0; i < min(3, (int)offenders.size()); i++) {
            cout << "  " << i + 1 << ". " << offenders[i].first
                << " - " << offenders[i].second << " нарушений" << endl;
        }
    }

    string getVehicleReport(const string& vehicleId) {
        auto it = violationsByVehicle.find(vehicleId);
        if (it == violationsByVehicle.end() || it->second.empty()) {
            return "Автомобиль " + vehicleId + " не имеет нарушений";
        }

        string report = "\n=== ОТЧЁТ ПО АВТОМОБИЛЮ " + vehicleId + " ===\n";
        report += "Всего нарушений: " + to_string(it->second.size()) + "\n";

        int totalFine = 0;
        report += "Список нарушений:\n";
        for (auto v : it->second) {
            report += "  - " + v->getDescription() + "\n";
            totalFine += v->getFine();
        }
        report += "Общая сумма штрафов: " + to_string(totalFine) + " руб.\n";

        return report;
    }

    // Очистка всех нарушений (если нужно)
    void clear() {
        for (auto v : allViolations) delete v;
        allViolations.clear();
        violationsByVehicle.clear();
        violationsByZone.clear();
        violationsByHour.clear();
    }
};