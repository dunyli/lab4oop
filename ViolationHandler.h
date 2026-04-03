#pragma once
#include "VideoHandler.h"
#include "ControlZone.h"
#include <iostream>
using namespace std;

class ViolationHandler : public VideoHandler {
private:
    ControlZone* zone;

public:
    // конструктор - принимает VideoSource* и ControlZone*
    ViolationHandler(VideoSource* src, ControlZone* z)
        : VideoHandler(src), zone(z) {}

    void process(Vehicle* v) override {
        cout << "\n=== ОБРАБОТЧИК НАРУШЕНИЙ ===" << endl;
        cout << "Источник: " << source->getSourceName() << endl;

        Frame* frame = source->getNextFrame();
        cout << "Кадр получен, timestamp=" << frame->timestamp << endl;

        // Используем зону для проверки
        if (zone) {
            zone->printCheck(v);
        }

        delete frame;
    }
};