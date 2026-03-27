#pragma once
#include <queue>
#include <vector>
#include "VideoProcessor.h"
#include "Frame.h"
#include "Vehicle.h"

using namespace std;

class VideoProcessorProxy : public VideoProcessor {
private:
    queue<Frame*> buffer;
    int maxSize;

public:
    VideoProcessorProxy(ControlZone* z, EvidenceCollector* ec, ResolutionGenerator* rg, int bufferSize);
    ~VideoProcessorProxy();

    void processFrame(Frame* frame, Vehicle* v, int quality, int confidence = 10) override;
    void processFrameWithFrames(Vehicle* v, int quality, int confidence, vector<Frame*>& frames) override;

    vector<Frame*> getAllFrames();
    int getBufferSize() const;
    void clearBuffer();
    void addToBuffer(Frame* frame);
};