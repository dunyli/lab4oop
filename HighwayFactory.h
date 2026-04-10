#pragma once
#include "AbstractZoneFactory.h"
#include "SpeedRuleCreator.h"
#include "EvidenceCollectorProxy.h"
#include "ResolutionProxy.h"
#include <iostream>

using namespace std;

class HighwayFactory : public AbstractZoneFactory {
public:
    ControlZone* createZone() override {
        ControlZone* zone = new ControlZone(2);

        RuleCreator* speedCreator = new SpeedRuleCreator(110);
        zone->addRule(speedCreator->createRule());
        delete speedCreator;

        cout << "[HighwayFactory] Создана зона с загородными правилами" << endl;
        return zone;
    }

    EvidenceCollector* createEvidenceCollector() override {
        return new EvidenceCollectorProxy(6, 2, 1);
    }

    ResolutionGenerator* createResolutionGenerator() override {
        // ResolutionProxy(требование метаданных, минимальная уверенность, автозапрос)
        return new ResolutionProxy(false, 6, false);
    }
};