#pragma once
#include <string>
using namespace std;

class LicensePlate {
public:
    string number;
    string region;

    LicensePlate(string n, string r) : number(n), region(r) {}

    string getFullNumber() {
        return number + " " + region;
    }
};