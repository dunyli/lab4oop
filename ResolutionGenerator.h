#pragma once
#include "Resolution.h"
#include "Evidence.h"
#include <ctime>

using namespace std;

class ResolutionGenerator {
public:
    virtual ~ResolutionGenerator() {}
    virtual Resolution* generate(Evidence* e, int speed, int confidence = 10);
};