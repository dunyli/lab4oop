#pragma once
#include "SpeedProvider.h"
#include "RadarSensor.h"

// Адаптер для радарного датчика
// Преобразует м/с - км/ч
class RadarAdapter : public SpeedProvider {
private:
    RadarSensor* radar;  // Ссылка на адаптируемый объект

public:
    // Конструктор принимает указатель на реальный датчик
    RadarAdapter(RadarSensor* r) : radar(r) {}

    // Реализация целевого интерфейса
    int getSpeedKmh() override {
        // 1. Получаем данные от датчика в его формате
        float speedMps = radar->getSpeedMps();

        // 2. Преобразуем в нужный формат
        // Формула: км/ч = м/с * 3.6
        int speedKmh = static_cast<int>(speedMps * 3.6);

        // 3. Возвращаем преобразованные данные
        return speedKmh;
    }
};
