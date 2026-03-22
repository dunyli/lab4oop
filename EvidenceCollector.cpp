#include "EvidenceCollector.h"
#include <iostream>

Evidence* EvidenceCollector::collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime) {
    string evidId = "EVID_" + to_string(time(nullptr));
    Evidence* e = new Evidence(evidId);
    for (auto f : frames) {
        e->addFrame(f);
    }
    return e;
}