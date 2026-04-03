#pragma once
#include "VideoSource.h"
#include "Vehicle.h"
#include <iostream>
using namespace std;

class VideoHandler {
protected:
    VideoSource* source;

public:
    VideoHandler(VideoSource* src) : source(src) {}
    virtual ~VideoHandler() {}

    virtual void process(Vehicle* v) = 0;

    void setSource(VideoSource* src) {
        source = src;
        cout << "[Handler] Сменён источник на " << source->getSourceName() << endl;
    }
};