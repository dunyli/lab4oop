#pragma once
#include <vector>
#include "Frame.h"

using namespace std;

class FrameBuffer {
public:
    virtual ~FrameBuffer() {}
    virtual void addFrame(Frame* frame) = 0;
    virtual vector<Frame*> getFrames() = 0;
    virtual int getSize() = 0;
    virtual void clear() = 0;
    virtual FrameBuffer* getComposite() { return nullptr; }
};