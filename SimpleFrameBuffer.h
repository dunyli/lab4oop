#pragma once
#include <vector>
#include <queue>
#include "FrameBuffer.h"
#include "Frame.h"

using namespace std;

class SimpleFrameBuffer : public FrameBuffer {
private:
    queue<Frame*> buffer;
    int maxSize;

public:
    SimpleFrameBuffer(int size) : maxSize(size) {}

    ~SimpleFrameBuffer() {
        clear();
    }

    void addFrame(Frame* frame) override {
        buffer.push(frame);
        if (buffer.size() > maxSize) {
            delete buffer.front();
            buffer.pop();
        }
    }

    vector<Frame*> getFrames() override {
        vector<Frame*> frames;
        queue<Frame*> temp = buffer;
        while (!temp.empty()) {
            frames.push_back(temp.front());
            temp.pop();
        }
        return frames;
    }

    int getSize() override {
        return buffer.size();
    }

    void clear() override {
        while (!buffer.empty()) {
            delete buffer.front();
            buffer.pop();
        }
    }
}; 
