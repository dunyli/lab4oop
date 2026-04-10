#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Frame.h"
#include "Violation.h"
using namespace std;

class Evidence {
private:
    string id;
    vector<Frame*> frames;

public:
    Evidence(string i) : id(i) {}

    void addFrame(Frame* f) {
        frames.push_back(f);
    }

    void save() {
        cout << "  [Evidence] Сохранены доказательства " << id
            << ", кадров: " << frames.size() << endl;
    }

    string getId() const { return id; }
    int getFramesCount() const { return frames.size(); }
};