#pragma once
#include "ControlZone.h"
#include "EvidenceCollector.h"
#include "ResolutionGenerator.h"

class AbstractZoneFactory {
public:
    virtual ~AbstractZoneFactory() {}
    virtual ControlZone* createZone() = 0;
    virtual EvidenceCollector* createEvidenceCollector() = 0;
    virtual ResolutionGenerator* createResolutionGenerator() = 0;
};