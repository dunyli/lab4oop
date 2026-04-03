#pragma once
#include "SpeedRule.h"
#include "BusLaneRule.h"
#include <map>
using namespace std;

class RuleFactory {
private:
    map<pair<int, string>, SpeedRule*> speedFlyweights;
    map<pair<int, string>, BusLaneRule*> busLaneFlyweights;

public:
    ~RuleFactory() {
        for (auto& p : speedFlyweights) delete p.second;
        for (auto& p : busLaneFlyweights) delete p.second;
    }

    SpeedRule* getSpeedRule(int limit, const string& zone) {
        auto key = make_pair(limit, zone);
        auto it = speedFlyweights.find(key);
        if (it != speedFlyweights.end()) return it->second;
        SpeedRule* r = new SpeedRule(limit, zone);
        speedFlyweights[key] = r;
        return r;
    }

    BusLaneRule* getBusLaneRule(int lane, const string& zone) {
        auto key = make_pair(lane, zone);
        auto it = busLaneFlyweights.find(key);
        if (it != busLaneFlyweights.end()) return it->second;
        BusLaneRule* r = new BusLaneRule(lane, zone);
        busLaneFlyweights[key] = r;
        return r;
    }

    int getSpeedRuleCount() const { return speedFlyweights.size(); }
    int getBusLaneRuleCount() const { return busLaneFlyweights.size(); }
};