#pragma once
#include "ViolationDecorator.h"
#include <iostream>

// Декоратор, добавляющий пометку "Срочное нарушение"
class PriorityViolation : public ViolationDecorator {
public:
    PriorityViolation(Violation* w) : ViolationDecorator(w) {}

    string getDescription() override {
        // Добавляем пометку к описанию
        return "СРОЧНО! " + wrapped->getDescription();
    }
};
