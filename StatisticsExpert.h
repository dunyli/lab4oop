#pragma once
#include "IViolationExpert.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <ctime>
using namespace std;

class StatisticsExpert : public IViolationExpert {
private:
    vector<Violation*> allViolations;
    map<string, vector<Violation*>> violationsByZone;
    map<string, vector<Violation*>> violationsByVehicle;

public:
    ~StatisticsExpert() { clear(); }

    void addViolation(Violation* v) override {
        allViolations.push_back(v);
        violationsByZone[v->zoneName].push_back(v);
        violationsByVehicle[v->vehicle->id].push_back(v);
    }

    int getTotalCount() override { return allViolations.size(); }

    void clear() override {
        for (auto v : allViolations) delete v;
        allViolations.clear();
        violationsByZone.clear();
        violationsByVehicle.clear();
    }

    // Главный метод: вывод статистики
    string getInfo() override {
        string result = "\n=== СТАТИСТИКА ===\n";
        result += "Всего нарушений: " + to_string(allViolations.size()) + "\n";

        result += "\nПо зонам:\n";
        for (auto& p : violationsByZone) {
            result += "  " + p.first + ": " + to_string(p.second.size()) + "\n";
        }

        result += "\nТоп нарушителей:\n";
        vector<pair<string, int>> offenders;
        for (auto& p : violationsByVehicle) {
            offenders.push_back({ p.first, p.second.size() });
        }
        sort(offenders.begin(), offenders.end(),
            [](auto& a, auto& b) { return a.second > b.second; });

        for (int i = 0; i < min(3, (int)offenders.size()); i++) {
            result += "  " + to_string(i + 1) + ". " + offenders[i].first +
                " - " + to_string(offenders[i].second) + "\n";
        }
        return result;
    }
};