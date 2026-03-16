#include "VideoProcessor.h"
#include <iostream>

using namespace std;

// Конструктор
VideoProcessor::VideoProcessor(ControlZone* z, EvidenceCollectorProxy* ep, ResolutionProxy* rp)
    : zone(z), evidenceProxy(ep), resolutionProxy(rp) {
    cout << "[VideoProcessor] Создан" << endl;
}

// Обработка кадра с одним текущим кадром
void VideoProcessor::processFrame(Frame* frame, Vehicle* v, int quality, int confidence) {
    cout << "\n[VideoProcessor] Скорость: " << v->speed << " км/ч, Полоса: "
        << v->lane << ", Тип ТС: " << v->getType() << endl;

    // ДЕЛЕГИРОВАНИЕ: передаем проверку нарушений в ControlZone
    auto violations = zone->check(v);

    // Обрабатываем каждое обнаруженное нарушение
    for (auto vio : violations) {
        cout << "  [VideoProcessor] Обнаружено нарушение: " << vio->getDescription() << endl;

        // ДЕЛЕГИРОВАНИЕ прокси для сбора доказательств
        vector<Frame*> frames = { frame };
        Evidence* e = evidenceProxy->collect(vio, frames, quality);

        if (e) {
            e->save();

            // ДЕЛЕГИРОВАНИЕ прокси для создания постановления
            Resolution* r = resolutionProxy->generate(e, v->speed, confidence);
            if (r) {
                r->save();
                delete r;
            }
            delete e;
        }
        delete vio;
    }
}

// Обработка нарушения с использованием готового набора кадров из буфера
void VideoProcessor::processFrameWithFrames(Vehicle* v, int quality, int confidence, vector<Frame*>& frames) {
    cout << "\n[VideoProcessor] Скорость: " << v->speed << " км/ч, Полоса: "
        << v->lane << ", Тип ТС: " << v->getType() << endl;

    // ДЕЛЕГИРОВАНИЕ: передаем проверку нарушений в ControlZone
    auto violations = zone->check(v);

    if (violations.empty()) {
        cout << "  [VideoProcessor] Нарушений не обнаружено" << endl;
    }

    // Обрабатываем каждое обнаруженное нарушение
    for (auto vio : violations) {
        cout << "  [VideoProcessor] Обнаружено нарушение: " << vio->getDescription() << endl;

        // ДЕЛЕГИРОВАНИЕ прокси для сбора доказательств
        Evidence* e = evidenceProxy->collect(vio, frames, quality);

        if (e) {
            cout << "  [VideoProcessor] Доказательства собраны" << endl;
            e->save();

            // ДЕЛЕГИРОВАНИЕ прокси для создания постановления
            Resolution* r = resolutionProxy->generate(e, v->speed, confidence);
            if (r) {
                cout << "  [VideoProcessor] Постановление сформировано" << endl;
                r->save();
                delete r;
            }
            else {
                cout << "  [VideoProcessor] Постановление не сформировано" << endl;
            }
            delete e;
        }
        else {
            cout << "  [VideoProcessor] Доказательства не собраны" << endl;
        }
        delete vio;
    }
}