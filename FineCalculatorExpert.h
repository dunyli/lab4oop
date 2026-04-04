#pragma once
#include "IViolationExpert.h"
#include <map>
#include <vector>
#include <iostream>
using namespace std;

class FineCalculatorExpert : public IViolationExpert {
private:
    vector<Violation*> allViolations;
    map<string, int> violationCountByVehicle;  // ñ÷¸ò÷èê íàðóøåíèé ïî àâòîìîáèëÿì

public:
    ~FineCalculatorExpert() { clear(); }

    void addViolation(Violation* v) override {
        allViolations.push_back(v);
        violationCountByVehicle[v->vehicle->id]++;
    }

    int getTotalCount() override { return allViolations.size(); }

    void clear() override {
        for (auto v : allViolations) delete v;
        allViolations.clear();
        violationCountByVehicle.clear();
    }

    // Ãëàâíûé ìåòîä: ðàñ÷¸ò øòðàôà ñ ó÷¸òîì èñòîðèè
    string getInfo() override {
        string result = "\nÐÀÑ×¨Ò ØÒÐÀÔÎÂ Ñ Ó×¨ÒÎÌ ÈÑÒÎÐÈÈ \n";

        for (auto& p : violationCountByVehicle) {
            int count = p.second;
            int baseFine = 500;  // áàçîâûé øòðàô
            int totalFine = baseFine * count;

            if (count >= 3) {
                totalFine = baseFine * count * 1.5;
                result += "  " + p.first + ": " + to_string(count) +
                    " íàðóøåíèé, ØÒÐÀÔ Ñ ÓÂÅËÈ×ÅÍÈÅÌ 50%: " + to_string(totalFine) + " ðóá.\n";
            }
            else if (count >= 2) {
                totalFine = baseFine * count * 1.2;
                result += "  " + p.first + ": " + to_string(count) +
                    " íàðóøåíèé, ØÒÐÀÔ Ñ ÓÂÅËÈ×ÅÍÈÅÌ 20%: " + to_string(totalFine) + " ðóá.\n";
            }
            else {
                result += "  " + p.first + ": " + to_string(count) +
                    " íàðóøåíèå, ØÒÐÀÔ: " + to_string(totalFine) + " ðóá.\n";
            }
        }
        return result;
    }

    // Äîïîëíèòåëüíûé ìåòîä äëÿ ðàñ÷¸òà øòðàôà îäíîãî íàðóøåíèÿ
    int calculateFine(Violation* v) {
        int count = violationCountByVehicle[v->vehicle->id];
        int baseFine = v->getFine();

        if (count >= 3) return baseFine * 1.5;
        if (count >= 2) return baseFine * 1.2;
        return baseFine;
    }
};