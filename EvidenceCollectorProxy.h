#pragma once
#include "EvidenceCollector.h"
#include <vector>

using namespace std;

/**
 * «ащитный заместитель дл€ сборщика доказательств
 * ¬ыполн€ет проверку качества и полноты набора кадров перед сбором доказательств
 */
class EvidenceCollectorProxy : public EvidenceCollector {
private:
    int minQuality;           // ћинимально допустимое качество кадров
    int framesBefore;         // “ребуемое количество кадров до нарушени€
    int framesAfter;          // “ребуемое количество кадров после нарушени€

public:
    /**
     *  онструктор защитного прокси
     * qualityThreshold - минимальное качество кадров (0-10)
     * framesBeforeCount - сколько кадров должно быть до нарушени€
     * framesAfterCount - сколько кадров должно быть после нарушени€
     */
    EvidenceCollectorProxy(int qualityThreshold, int framesBeforeCount, int framesAfterCount);

    /**
     * —бор доказательств с предварительной проверкой качества
     * violation - объект нарушени€
     * frames - вектор кадров видеопотока
     * quality - качество кадров
     * violationTime - временна€ метка нарушени€
     */
    Evidence* collect(Violation* violation, vector<Frame*>& frames, int quality, long violationTime = 0) override;

    // √еттеры дл€ отладки и конфигурации
    int getMinQuality() const { return minQuality; }
    int getFramesBefore() const { return framesBefore; }
    int getFramesAfter() const { return framesAfter; }
};