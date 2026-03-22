#include "VideoProcessor.h"
#include <iostream>

using namespace std;

VideoProcessor::VideoProcessor(ControlZone* z, EvidenceCollector* ec, ResolutionGenerator* rg)
    : zone(z), evidenceCollector(ec), resolutionGenerator(rg) {
    cout << "[VideoProcessor] Создан" << endl;
}

VideoProcessor::~VideoProcessor() {
    cout << "[VideoProcessor] Уничтожен" << endl;
}

void VideoProcessor::processFrame(Frame* frame, Vehicle* v, int quality, int confidence) {
    cout << "\n[VideoProcessor] Скорость: " << v->speed << " км/ч, Полоса: "
        << v->lane << ", Тип ТС: " << v->getType() << endl;

    auto violations = zone->check(v);

    for (auto vio : violations) {
        cout << "  [VideoProcessor] Обнаружено нарушение: " << vio->getDescription() << endl;

        vector<Frame*> frames = { frame };
        Evidence* e = evidenceCollector->collect(vio, frames, quality);

        if (e) {
            e->save();
            Resolution* r = resolutionGenerator->generate(e, v->speed, confidence);
            if (r) {
                r->save();
                delete r;
            }
            delete e;
        }
        delete vio;
    }
}

void VideoProcessor::processFrameWithFrames(Vehicle* v, int quality, int confidence, vector<Frame*>& frames) {
    cout << "\n[VideoProcessor] Скорость: " << v->speed << " км/ч, Полоса: "
        << v->lane << ", Тип ТС: " << v->getType() << endl;

    auto violations = zone->check(v);

    if (violations.empty()) {
        cout << "  [VideoProcessor] Нарушений не обнаружено" << endl;
    }

    for (auto vio : violations) {
        cout << "  [VideoProcessor] Обнаружено нарушение: " << vio->getDescription() << endl;

        Evidence* e = evidenceCollector->collect(vio, frames, quality);

        if (e) {
            cout << "  [VideoProcessor] Доказательства собраны" << endl;
            e->save();

            Resolution* r = resolutionGenerator->generate(e, v->speed, confidence);
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