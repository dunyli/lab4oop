#pragma once
#include "FrameBuffer.h"
#include "Frame.h"
#include <list>

class CompositeFrameBuffer : public FrameBuffer {
private:
    list<FrameBuffer*> children;

public:
    ~CompositeFrameBuffer() {
        for (auto buf : children) delete buf;
    }

    void addBuffer(FrameBuffer* buffer) {
        children.push_back(buffer);
    }

    void removeBuffer(FrameBuffer* buffer) {
        children.remove(buffer);
    }

    FrameBuffer* getComposite() override {
        return this;
    }

    void addFrame(Frame* frame) override {
        // Добавляем кадр во все буферы
        for (auto buf : children) {
            buf->addFrame(frame);
        }
    }

    vector<Frame*> getFrames() override {
        vector<Frame*> allFrames;

        // Собираем кадры из всех буферов
        for (auto buf : children) {
            auto frames = buf->getFrames();
            allFrames.insert(allFrames.end(), frames.begin(), frames.end());
        }

        return allFrames;
    }

    int getSize() override {
        int total = 0;
        for (auto buf : children) {
            total += buf->getSize();
        }
        return total;
    }

    void clear() override {
        for (auto buf : children) {
            buf->clear();
        }
    }
};