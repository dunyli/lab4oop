#pragma once
#include "Evidence.h"
#include <vector>

using namespace std;

// Класс-прокси для сбора доказательств с проверкой качества
class EvidenceCollectorProxy {
public:
    int minQuality;      // Минимальный порог качества кадра (0-10)
    int framesBefore;    // Сколько кадров должно быть ДО нарушения
    int framesAfter;     // Сколько кадров должно быть ПОСЛЕ нарушения

    // Конструктор с параметрами настройки
    EvidenceCollectorProxy(int q, int before = 3, int after = 3);

    // Метод сбора доказательств с проверкой качества и количества кадров
    Evidence* collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime = 0);
};