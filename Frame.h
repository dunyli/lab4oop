#pragma once
#include <iostream>
using namespace std;

class Frame {
public:
    long timestamp;

    Frame(long t = 0) : timestamp(t) {}

    void reset(long t) {
        timestamp = t;
    }

    void show() {
        cout << "  [Frame] timestamp: " << timestamp << endl;
    }
};