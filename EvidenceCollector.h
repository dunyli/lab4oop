#pragma once
#include "Evidence.h"
#include "Violation.h"
#include <vector>
#include <ctime>

using namespace std;

// Базовый класс для сборщика доказательств
class EvidenceCollector {
public:
    virtual ~EvidenceCollector() {}

    // Виртуальный метод сбора доказательств
    virtual Evidence* collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime = 0);
};