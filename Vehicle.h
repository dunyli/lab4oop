#pragma once
#include <string>
using namespace std;

class Vehicle {
public:
    string id;
    int speed;
    int lane;
    string type;

    Vehicle(const string& i, const string& t) : id(i), type(t), speed(0), lane(0) {}
    virtual ~Vehicle() {}
    string getType() const { return type; }
};