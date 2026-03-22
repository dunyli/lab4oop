#include "VideoProcessorProxy.h"
#include <iostream>

using namespace std;

VideoProcessorProxy::VideoProcessorProxy(ControlZone* z, EvidenceCollector* ec, ResolutionGenerator* rg, int bufferSize)
    : VideoProcessor(z, ec, rg), maxSize(bufferSize) {
    cout << "[VideoProcessorProxy] Создан с размером буфера: " << maxSize << " кадров" << endl;
}

VideoProcessorProxy::~VideoProcessorProxy() {
    cout << "[VideoProcessorProxy] Очистка буфера, удаляется " << buffer.size() << " кадров" << endl;
    while (!buffer.empty()) {
        delete buffer.front();
        buffer.pop();
    }
}

void VideoProcessorProxy::processFrame(Frame* frame, Vehicle* v, int quality, int confidence) {
    buffer.push(frame);
    if (buffer.size() > maxSize) {
        delete buffer.front();
        buffer.pop();
    }
    cout << "[VideoProcessorProxy] Кадр добавлен в буфер. Размер: " << buffer.size() << "/" << maxSize << endl;
    VideoProcessor::processFrame(frame, v, quality, confidence);
}

void VideoProcessorProxy::processFrameWithFrames(Vehicle* v, int quality, int confidence, vector<Frame*>& frames) {
    if (frames.empty()) {
        frames = getAllFrames();
    }
    VideoProcessor::processFrameWithFrames(v, quality, confidence, frames);
}

vector<Frame*> VideoProcessorProxy::getAllFrames() {
    vector<Frame*> frames;
    queue<Frame*> temp = buffer;
    while (!temp.empty()) {
        frames.push_back(temp.front());
        temp.pop();
    }
    cout << "[VideoProcessorProxy] Получено " << frames.size() << " кадров из буфера" << endl;
    return frames;
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

void VideoProcessorProxy::addToBuffer(Frame* frame) {
    buffer.push(frame);
    if (buffer.size() > maxSize) {
        delete buffer.front();
        buffer.pop();
    }
}