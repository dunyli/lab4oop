#pragma once
#include "ControlZone.h"
#include "Rule.h"

class ControlZoneWithPrototype : public ControlZone {
public:
    // Обычный конструктор
    ControlZoneWithPrototype(int id = 0) : ControlZone(id) {}

    // Конструктор копирования для клонирования
    ControlZoneWithPrototype(const ControlZoneWithPrototype& other)
        : ControlZone(other.zoneId + 1000) {  // передаём новый ID в базовый класс
        // Копируем правила из другого объекта
        for (auto r : other.rules) {
            rules.push_back(r);
        }
    }

    // Метод клонирования
    ControlZoneWithPrototype* clone() {
        return new ControlZoneWithPrototype(*this);
    }

    // Метод для добавления правила (для удобства)
    void addRuleDirect(Rule* rule) {
        rules.push_back(rule);
    }
};