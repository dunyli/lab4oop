#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Frame.h"
#include "Violation.h"
using namespace std;

class Evidence {
public:
    string id;
    vector<Frame*> frames;

    Evidence(string i) : id(i) {}

    void addFrame(Frame* f) {
        frames.push_back(f);
    }

    void save() {
        cout << " [Evidence] Сохранены доказательства " << id.c_str() << ", кадров: " << (int)frames.size() << endl; 
    }
};