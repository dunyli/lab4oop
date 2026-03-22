#pragma once
#include "ResolutionGenerator.h"

using namespace std;

class ResolutionProxy : public ResolutionGenerator {
private:
    bool requireMetadata;
    int minConfidence;
    bool autoRequest;

public:
    ResolutionProxy(bool meta, int conf = 9, bool autoReq = false);
    Resolution* generate(Evidence* e, int speed, int confidence = 10) override;

    // Геттеры
    bool getRequireMetadata() const { return requireMetadata; }
    int getMinConfidence() const { return minConfidence; }
    bool getAutoRequest() const { return autoRequest; }
};