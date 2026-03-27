#pragma once
class Frame {
public:
    long timestamp;
    Frame(long t) : timestamp(t) {}
    long getTimestamp() const {
        return timestamp;
    }
};