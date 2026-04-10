#pragma once
#include <queue>
#include <vector>
#include "VideoProcessor.h"

using namespace std;

class VideoProcessorProxy : public VideoProcessor {
private:
    queue<Frame*> buffer;
    int maxSize;

public:
    VideoProcessorProxy(ControlZone* controlZone, EvidenceCollector* collector,
        ResolutionGenerator* generator, int ringBufferSize);
    ~VideoProcessorProxy();

    void processFrame(Frame* incomingFrame, Vehicle* vehicle, int frameQuality, int ocrConfidence = 10) override;
    void processFrameWithFrames(Vehicle* vehicle, int frameQuality, int ocrConfidence,
        vector<Frame*>& externalFrames) override;

    // мнбши лернд processVehicle
    void processVehicle(Vehicle* vehicle, int frameQuality, int ocrConfidence, vector<Frame*>& frames) override;

    vector<Frame*> getAllFrames();
    int getBufferSize() const;
    void clearBuffer();
    void addToBuffer(Frame* newFrame);
};