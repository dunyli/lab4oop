#pragma once
#include "Evidence.h"
#include "Violation.h"
#include <vector>

using namespace std;

/**
 * Базовый класс сборщика доказательств
 * Отвечает за создание контейнера с доказательствами по факту нарушения
 */
class EvidenceCollector {
public:
    virtual ~EvidenceCollector() {}

    /*
     * Сбор доказательств по нарушению
     * v - объект нарушения
     * frames - вектор кадров видеопотока
     * quality - качество кадров (0-10)
     * violationTime - временная метка нарушения
     */
    virtual Evidence* collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime = 0);
};