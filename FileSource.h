#pragma once
#include "VideoSource.h"
#include <iostream>
using namespace std;

class FileSource : public VideoSource {
private:
    string filePath;
    int frameCounter;

public:
    FileSource(const string& path) : filePath(path), frameCounter(0) {}

    Frame* getNextFrame() override {
        frameCounter++;
        cout << "[FileSource " << filePath << "] Чтение кадра #" << frameCounter << endl;
        return new Frame(time(nullptr));
    }

    string getSourceName() override {
        return "FileSource_" + filePath;
    }
};