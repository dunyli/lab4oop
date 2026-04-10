#pragma once
#include "Resolution.h"
#include "Evidence.h"

using namespace std;

/**
 * Базовый класс генератора постановлений о штрафе
 * Выполняет расчет суммы штрафа на основе величины превышения скорости
 */
class ResolutionGenerator {
public:
    virtual ~ResolutionGenerator() {}

    /**
     * Формирование постановления о штрафе
     * evidence - доказательства нарушения
     * vehicleSpeed - скорость транспортного средства
     * recognitionConfidence - уверенность распознавания номера (0-10)
     */
    virtual Resolution* generate(Evidence* evidence, int vehicleSpeed, int recognitionConfidence = 10);
};