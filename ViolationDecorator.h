#pragma once
#include "Violation.h"

// Базовый декоратор для нарушений
class ViolationDecorator : public Violation {
protected:
    Violation* wrapped;  // декорируемое нарушение

public:
    // Конструктор принимает нарушение для декорирования
    ViolationDecorator(Violation* w) : wrapped(w) {}

    // Деструктор очищает память (как в примере)
    virtual ~ViolationDecorator() {
        delete wrapped;
    }

    // Чисто виртуальный метод — заставляем наследников реализовать
    virtual string getDescription() = 0;
};