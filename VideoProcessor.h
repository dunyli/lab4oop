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
    VideoProcessor(ControlZone* controlZone, EvidenceCollector* collector, ResolutionGenerator* generator);
    virtual ~VideoProcessor();

    // Существующие методы
    virtual void processFrame(Frame* currentFrame, Vehicle* vehicle, int frameQuality, int ocrConfidence = 10);
    virtual void processFrameWithFrames(Vehicle* vehicle, int frameQuality, int ocrConfidence,
        vector<Frame*>& preCapturedFrames);

    // НОВЫЙ МЕТОД processVehicle
    virtual void processVehicle(Vehicle* vehicle, int frameQuality, int ocrConfidence, vector<Frame*>& frames);

    // Геттеры
    ControlZone* getZone() const { return zone; }
    EvidenceCollector* getEvidenceCollector() const { return evidenceCollector; }
    ResolutionGenerator* getResolutionGenerator() const { return resolutionGenerator; }
};