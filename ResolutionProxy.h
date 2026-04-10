#pragma once
#include "ResolutionGenerator.h"

using namespace std;

class ResolutionProxy : public ResolutionGenerator {
private:
    bool requireMetadata;
    int minConfidence;
    bool autoRequest;

public:
    // Конструктор с 3 параметрами
    ResolutionProxy(bool requireMetadataCheck, int minConfidenceLevel, bool enableAutoRequest);

    Resolution* generate(Evidence* evidence, int vehicleSpeed, int recognitionConfidence = 10) override;

    // Геттеры
    bool getRequireMetadata() const { return requireMetadata; }
    int getMinConfidence() const { return minConfidence; }
    bool getAutoRequest() const { return autoRequest; }
};