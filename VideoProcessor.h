#pragma once
#include "ControlZone.h"
#include "EvidenceCollectorProxy.h"
#include "ResolutionProxy.h"
#include <vector>

using namespace std;

// Главный класс видеопроцессора, который координирует всю работу системы
class VideoProcessor {
public:
    ControlZone* zone;                 // Зона контроля с правилами
    EvidenceCollectorProxy* evidenceProxy; // Прокси для сбора доказательств
    ResolutionProxy* resolutionProxy;   // Прокси для формирования постановлений

    // Конструктор
    VideoProcessor(ControlZone* z, EvidenceCollectorProxy* ep, ResolutionProxy* rp);

    // Обработка кадра с одним текущим кадром
    void processFrame(Frame* frame, Vehicle* v, int quality, int confidence = 10);

    // Обработка нарушения с использованием готового набора кадров из буфера
    void processFrameWithFrames(Vehicle* v, int quality, int confidence, vector<Frame*>& frames);
};