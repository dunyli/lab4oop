#include "ResolutionGenerator.h"
#include <iostream>

Resolution* ResolutionGenerator::generate(Evidence* e, int speed, int confidence) {
    int amount = 500;
    if (speed > 80) amount = 1500;
    else if (speed > 70) amount = 1000;

    string resId = "RES_" + to_string(time(nullptr));
    return new Resolution(resId, amount);
}