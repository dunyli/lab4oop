// FrameIterator.h
#pragma once
#include "Frame.h"
#include <vector>

using namespace std;

class FrameIterator {
private:
    vector<Frame*>::iterator current;
    vector<Frame*>::iterator end;

public:
    FrameIterator(vector<Frame*>::iterator begin, vector<Frame*>::iterator endIt)
        : current(begin), end(endIt) {}

    bool hasNext() {
        return current != end;
    }

    Frame* next() {
        if (current == end) return nullptr;
        Frame* frame = *current;
        current++;
        return frame;
    }

    FrameIterator& operator++() {
        if (current != end) ++current;
        return *this;
    }

    FrameIterator operator++(int) {
        FrameIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const FrameIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const FrameIterator& other) const {
        return !(*this == other);
    }

    Frame* operator*() {
        if (current == end) return nullptr;
        return *current;
    }
};