#pragma once
#include "Rule.h"

// Абстрактный класс-создатель
class RuleCreator {
public:
    virtual ~RuleCreator() {}
    virtual Rule* createRule() = 0;
};