#pragma once
#include "ViolationDecorator.h"
#include <iostream>

// Декоратор, добавляющий комментарий к нарушению
class DescriptionViolation : public ViolationDecorator {
private:
    string comment;

public:
    DescriptionViolation(Violation* w, string c) : ViolationDecorator(w), comment(c) {}

    string getDescription() override {
        // Добавляем комментарий к описанию
        return wrapped->getDescription() + " (" + comment + ")";
    }
};