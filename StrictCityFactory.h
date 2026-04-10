#pragma once
#include "AbstractZoneFactory.h"
#include "SpeedRuleCreator.h"
#include "TrafficLightRule.h"
#include "LineCrossRule.h"
#include "BusLaneRuleCreator.h"
#include "EvidenceCollectorProxy.h"
#include "ResolutionProxy.h"
#include <iostream>

using namespace std;

class StrictCityFactory : public AbstractZoneFactory {
public:
    ControlZone* createZone() override {
        ControlZone* zone = new ControlZone(1);
        //Создаем зону с ограничением скорости 60
        RuleCreator* speedCreator = new SpeedRuleCreator(60);
        zone->addRule(speedCreator->createRule());
        delete speedCreator;

        zone->addRule(new TrafficLightRule());
        zone->addRule(new LineCrossRule());

        RuleCreator* busCreator = new BusLaneRuleCreator(1);
        zone->addRule(busCreator->createRule());
        delete busCreator;

        cout << "[StrictCityFactory] Создана зона с жёсткими правилами" << endl;
        return zone;
    }

    EvidenceCollector* createEvidenceCollector() override {
        return new EvidenceCollectorProxy(9, 3, 3);
    }

    ResolutionGenerator* createResolutionGenerator() override {
        // ResolutionProxy(требование метаданных, минимальная уверенность, автозапрос)
        return new ResolutionProxy(true, 9, false);
    }
};