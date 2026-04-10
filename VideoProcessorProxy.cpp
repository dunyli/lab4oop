#include "VideoProcessorProxy.h"
#include "Frame.h"
#include <iostream>

using namespace std;

VideoProcessorProxy::VideoProcessorProxy(ControlZone* controlZone, EvidenceCollector* collector,
    ResolutionGenerator* generator, int ringBufferSize)
    : VideoProcessor(controlZone, collector, generator), maxSize(ringBufferSize) {
    cout << "[VideoProcessorProxy] Фильтрующий прокси инициализирован: буфер на " << ringBufferSize << " кадров" << endl;
}

VideoProcessorProxy::~VideoProcessorProxy() {
    cout << "[VideoProcessorProxy] Освобождение буфера, удаляется " << buffer.size() << " кадров" << endl;
    while (!buffer.empty()) {
        delete buffer.front();
        buffer.pop();
    }
}

void VideoProcessorProxy::processFrame(Frame* incomingFrame, Vehicle* vehicle, int frameQuality, int ocrConfidence) {
    buffer.push(incomingFrame);
    if (buffer.size() > maxSize) {
        delete buffer.front();
        buffer.pop();
    }
    cout << "[VideoProcessorProxy] Кадр добавлен в буфер. Размер: " << buffer.size() << "/" << maxSize << endl;
    VideoProcessor::processFrame(incomingFrame, vehicle, frameQuality, ocrConfidence);
}

void VideoProcessorProxy::processFrameWithFrames(Vehicle* vehicle, int frameQuality, int ocrConfidence,
    vector<Frame*>& externalFrames) {
    if (externalFrames.empty()) {
        externalFrames = getAllFrames();
    }
    VideoProcessor::processFrameWithFrames(vehicle, frameQuality, ocrConfidence, externalFrames);
}

// НОВАЯ РЕАЛИЗАЦИЯ processVehicle
void VideoProcessorProxy::processVehicle(Vehicle* vehicle, int frameQuality, int ocrConfidence, vector<Frame*>& frames) {
    if (frames.empty()) {
        frames = getAllFrames();
    }
    VideoProcessor::processVehicle(vehicle, frameQuality, ocrConfidence, frames);
}

vector<Frame*> VideoProcessorProxy::getAllFrames() {
    vector<Frame*> allFrames;
    queue<Frame*> tempBuffer = buffer;
    while (!tempBuffer.empty()) {
        allFrames.push_back(tempBuffer.front());
        tempBuffer.pop();
    }
    cout << "[VideoProcessorProxy] Извлечено " << allFrames.size() << " кадров из буфера" << endl;
    return allFrames;
}

int VideoProcessorProxy::getBufferSize() const {
    return buffer.size();
}

void VideoProcessorProxy::clearBuffer() {
    while (!buffer.empty()) {
        delete buffer.front();
        buffer.pop();
    }
    cout << "[VideoProcessorProxy] Буфер очищен" << endl;
}

void VideoProcessorProxy::addToBuffer(Frame* newFrame) {
    buffer.push(newFrame);
    if (buffer.size() > maxSize) {
        delete buffer.front();
        buffer.pop();
    }
}