#pragma once
#include "VideoHandler.h"
#include <vector>
#include <iostream>
using namespace std;

class ArchiveHandler : public VideoHandler {
private:
    vector<Frame*> archive;

public:
    ArchiveHandler(VideoSource* src) : VideoHandler(src) {}

    void process(Vehicle* v) override {
        cout << "\nАРХИВАТОР" << endl;
        cout << "Источник: " << source->getSourceName() << endl;

        Frame* frame = source->getNextFrame();
        cout << "Сохранение кадра " << frame->timestamp << " в архив" << endl;

        archive.push_back(frame);
        cout << "Всего в архиве: " << archive.size() << " кадров" << endl;
        cout << "Информация о ТС: " << v->id << " (" << v->type << ")" << endl;
    }

    ~ArchiveHandler() {
        for (auto f : archive) delete f;
    }
};