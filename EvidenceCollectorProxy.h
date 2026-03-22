#pragma once
#include "EvidenceCollector.h"
#include <vector>

using namespace std;

class EvidenceCollectorProxy : public EvidenceCollector {
private:
    int minQuality;
    int framesBefore;
    int framesAfter;

public:
    EvidenceCollectorProxy(int q, int before = 3, int after = 3);
    Evidence* collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime = 0) override;

    // Геттеры
    int getMinQuality() const { return minQuality; }
    int getFramesBefore() const { return framesBefore; }
    int getFramesAfter() const { return framesAfter; }
};