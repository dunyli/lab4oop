#pragma once
#include "SpeedProvider.h"
#include "LaserSensor.h"
// Адаптер для лазерного измерителя
// Преобразует миль/ч - км/ч
class LaserAdapter : public SpeedProvider {
private:
    LaserSensor* laser;  // Ссылка на адаптируемый объект

public:
    // Конструктор принимает указатель на реальный датчик
    LaserAdapter(LaserSensor* l) : laser(l) {}

    // Реализация целевого интерфейса
    int getSpeedKmh() override {
        // 1. Получаем данные от датчика в его формате
        int speedMph = laser->getSpeedMph();

        // 2. Преобразуем в нужный формат
        // Формула: км/ч = миль/ч - 1.60934
        int speedKmh = static_cast<int>(speedMph * 1.60934);

        // 3. Возвращаем преобразованные данные
        return speedKmh;
    }
};
