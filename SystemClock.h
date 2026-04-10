#pragma once
#include <ctime>
#include <iostream>
using namespace std;

class SystemClock {
private:
    static SystemClock* instance;
    long startTime;

    SystemClock() {
        startTime = time(nullptr);
        cout << "[SystemClock] Запущен глобальный таймер синхронизации" << endl;
    }

public:
    static SystemClock* getInstance() {
        if (instance == nullptr) {
            instance = new SystemClock();
        }
        return instance;
    }

    long getCurrentTimestamp() {
        return time(nullptr) - startTime;
    }

    long getRealTime() {
        return time(nullptr);
    }

    static void destroy() {
        delete instance;
        instance = nullptr;
    }
};

SystemClock* SystemClock::instance = nullptr;