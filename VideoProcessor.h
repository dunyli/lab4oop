#pragma once
#include "ControlZone.h"
#include "EvidenceCollector.h"
#include "ResolutionGenerator.h"
#include <vector>

using namespace std;

class VideoProcessor {
protected:
    ControlZone* zone;
    EvidenceCollector* evidenceCollector;
    ResolutionGenerator* resolutionGenerator;

public:
    VideoProcessor(ControlZone* z, EvidenceCollector* ec, ResolutionGenerator* rg);
    virtual ~VideoProcessor();

    virtual void processFrame(Frame* frame, Vehicle* v, int quality, int confidence = 10);
    virtual void processFrameWithFrames(Vehicle* v, int quality, int confidence, vector<Frame*>& frames);

    ControlZone* getZone() const { return zone; }
    EvidenceCollector* getEvidenceCollector() const { return evidenceCollector; }
    ResolutionGenerator* getResolutionGenerator() const { return resolutionGenerator; }
};