#pragma once
#include "Violation.h"
#include <string>
using namespace std;

// Интерфейс информационного эксперта
class IViolationExpert {
public:
    virtual ~IViolationExpert() {}

    // Основные методы
    virtual void addViolation(Violation* v) = 0;
    virtual int getTotalCount() = 0;
    virtual void clear() = 0;

    // Специфичные методы для каждого эксперта
    virtual string getInfo() = 0;
};