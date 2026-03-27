// EvidenceCollector.cpp
#include "EvidenceCollector.h"
#include "Evidence.h"
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

Evidence* EvidenceCollector::collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime) {
    string evidId = "EVID_" + to_string(time(nullptr));
    Evidence* e = new Evidence(evidId);

    // ВАЖНО: НЕ сохраняем указатели на кадры, а создаем копии
    // или просто не сохраняем их для демонстрации
    for (auto f : frames) {
        // Создаем копию кадра, чтобы не зависеть от оригиналов
        Frame* copy = new Frame(f->timestamp);
        e->addFrame(copy);
    }

    return e;
}