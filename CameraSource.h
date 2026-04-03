#pragma once
#include "VideoSource.h"
#include "Camera.h"
#include <iostream>
using namespace std;

class CameraSource : public VideoSource {
private:
    Camera* camera;
    string cameraId;

public:
    CameraSource(const string& id, Camera* cam) : cameraId(id), camera(cam) {}

    Frame* getNextFrame() override {
        cout << "[CameraSource " << cameraId << "] Запрос кадра" << endl;
        return camera->getFrame();
    }

    string getSourceName() override {
        return "CameraSource_" + cameraId;
    }
};