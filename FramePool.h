#pragma once
#include "Frame.h"
#include <vector>
#include <iostream>
using namespace std;

class FramePool {
private:
    // –еализован как синглтон, так как пул должен быть единственным
    static FramePool* instance;
    vector<Frame*> pool; // —¬ќЅќƒЌџ≈ кадры (ждут использовани€)
    vector<Frame*> allFrames; // ¬—≈ созданные кадры (дл€ очистки)
    int maxSize; // ћаксимальный размер пула
    int createdCount; // —колько кадров уже создано

    FramePool(int max = 100) : maxSize(max), createdCount(0) {
        cout << "[FramePool] —оздан пул на " << maxSize << " кадров" << endl;
    }

public:
    static FramePool* getInstance(int max = 100) {
        if (instance == nullptr) {
            instance = new FramePool(max);
        }
        return instance;
    }
    // ¬«я“№ кадр из пула
    Frame* acquireFrame(long timestamp) {
        // если есть свободный кадр в pool, извлекает его, сбрасывает timestamp и возвращает
        if (!pool.empty()) {
            Frame* f = pool.back();
            pool.pop_back();
            f->reset(timestamp);
            return f;
        }
        // если пул пуст, но не превышен maxSize, создаЄт новый кадр
        if (createdCount < maxSize) {
            createdCount++;
            Frame* f = new Frame(timestamp);
            allFrames.push_back(f);
            return f;
        }
        // если пул достиг максимума, возвращает nullptr
        cout << "  [FramePool] ¬нимание: пул пуст!" << endl;
        return nullptr;
    }
    // возвращает кадр обратно в пул 
    void releaseFrame(Frame* f) {
        if (f != nullptr) {
            pool.push_back(f);
        }
    }
    // выводит статистику использовани€ пула
    void showStats() {
        cout << "  [FramePool] —оздано: " << createdCount
            << ", в пуле: " << pool.size() << endl;
    }

    ~FramePool() {
        for (auto f : allFrames) delete f;
    }

    static void destroy() {
        delete instance;
        instance = nullptr;
    }
};

FramePool* FramePool::instance = nullptr;