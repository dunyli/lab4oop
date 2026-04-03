#pragma once
#include "Frame.h"
#include <string>
using namespace std;

class VideoSource {
public:
    virtual ~VideoSource() {}
    virtual Frame* getNextFrame() = 0;
    virtual string getSourceName() = 0;
};