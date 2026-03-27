#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Frame.h"
#include "Violation.h"
#include "FrameIterator.h"

using namespace std;

class Evidence {
public:
    string id;
    vector<Frame*> frames;

    Evidence(string i) : id(i) {}

    ~Evidence() {
        for (auto f : frames) {
            delete f;
        }
        frames.clear();
    }

    void addFrame(Frame* f) {
        frames.push_back(f);
    }

    void save() {
        cout << " [Evidence] Сохранены доказательства " << id << ", кадров: " << (int)frames.size() << endl;
    }

    // Возвращает итератор на начало вектора кадров
    FrameIterator begin() {
        return FrameIterator(frames.begin(), frames.end());
    }

    // Возвращает итератор на конец вектора кадров
    FrameIterator end() {
        return FrameIterator(frames.end(), frames.end());
    }

    // Получить количество кадров
    int getFrameCount() const {
        return (int)frames.size();
    }

    // Получить все кадры
    const vector<Frame*>& getFrames() const {
        return frames;
    }
};