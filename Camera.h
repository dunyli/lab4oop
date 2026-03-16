#pragma once
#include "Frame.h"
class Camera {
public:
    Frame* getFrame() { return new Frame(time(nullptr)); }
};