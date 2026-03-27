#pragma once
// Целевой интерфейс, который ожидает система
// Все адаптеры будут реализовывать этот интерфейс
class SpeedProvider {
public:
    virtual ~SpeedProvider() {}

    // Возвращает скорость в километрах в час (км/ч)
    virtual int getSpeedKmh() = 0;
};