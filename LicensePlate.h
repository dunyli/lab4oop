#pragma once
#include <string>

using namespace std;

class LicensePlate {
private:
    string number;
    string region;

public:
    //  онструктор с одним параметром (только номер)
    LicensePlate(string n) : number(n), region("") {}

    //  онструктор с двум€ параметрами (номер и регион)
    LicensePlate(string n, string r) : number(n), region(r) {}

    string getFullNumber() const {
        if (region.empty()) {
            return number;
        }
        return number + " " + region;
    }

    string getNumber() const { return number; }
    string getRegion() const { return region; }
};