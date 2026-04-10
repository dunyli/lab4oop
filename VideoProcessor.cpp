#include "VideoProcessor.h"
#include "Evidence.h"
#include "Resolution.h"
#include "Violation.h"
#include <iostream>

using namespace std;

VideoProcessor::VideoProcessor(ControlZone* controlZone, EvidenceCollector* collector, ResolutionGenerator* generator)
    : zone(controlZone), evidenceCollector(collector), resolutionGenerator(generator) {
    cout << "[VideoProcessor] Центральный процессор видеопотока запущен" << endl;
}

VideoProcessor::~VideoProcessor() {
    cout << "[VideoProcessor] Остановка процессора видеопотока" << endl;
}

void VideoProcessor::processFrame(Frame* currentFrame, Vehicle* vehicle, int frameQuality, int ocrConfidence) {
    cout << "\n[VideoProcessor] Анализ транспортного средства: скорость=" << vehicle->speed
        << " км/ч, полоса=" << vehicle->lane
        << ", тип=" << vehicle->getType() << endl;

    auto violations = zone->check(vehicle);

    for (auto violation : violations) {
        cout << "  [VideoProcessor] ЗАФИКСИРОВАНО НАРУШЕНИЕ: " << violation->getDescription() << endl;

        vector<Frame*> evidenceFrames = { currentFrame };
        Evidence* evidence = evidenceCollector->collect(violation, evidenceFrames, frameQuality);

        if (evidence) {
            evidence->save();
            Resolution* resolution = resolutionGenerator->generate(evidence, vehicle->speed, ocrConfidence);
            if (resolution) {
                resolution->save();
                delete resolution;
            }
            delete evidence;
        }
        delete violation;
    }
}

void VideoProcessor::processFrameWithFrames(Vehicle* vehicle, int frameQuality, int ocrConfidence, vector<Frame*>& preCapturedFrames) {
    cout << "\n[VideoProcessor] Комплексный анализ ТС: скорость=" << vehicle->speed
        << " км/ч, полоса=" << vehicle->lane
        << ", тип=" << vehicle->getType() << endl;

    auto violations = zone->check(vehicle);

    if (violations.empty()) {
        cout << "  [VideoProcessor] Нарушений правил дорожного движения не обнаружено" << endl;
    }

    for (auto violation : violations) {
        cout << "  [VideoProcessor] ЗАФИКСИРОВАНО НАРУШЕНИЕ: " << violation->getDescription() << endl;
        cout << "  [VideoProcessor] Количество кадров в доказательной базе: " << preCapturedFrames.size() << endl;

        Evidence* evidence = new Evidence("DEMO_" + to_string(time(nullptr)));

        if (evidence) {
            cout << "  [VideoProcessor] Доказательства успешно собраны" << endl;
            evidence->save();

            Resolution* resolution = resolutionGenerator->generate(evidence, vehicle->speed, ocrConfidence);
            if (resolution) {
                cout << "  [VideoProcessor] Постановление о штрафе сформировано" << endl;
                resolution->save();
                delete resolution;
            }
            else {
                cout << "  [VideoProcessor] Отказ в формировании постановления" << endl;
            }
            delete evidence;
        }
        else {
            cout << "  [VideoProcessor] Не удалось собрать доказательства" << endl;
        }
        delete violation;
    }
}

// НОВАЯ РЕАЛИЗАЦИЯ processVehicle
void VideoProcessor::processVehicle(Vehicle* vehicle, int frameQuality, int ocrConfidence, vector<Frame*>& frames) {
    cout << "\n[VideoProcessor] Обработка ТС: " << vehicle->id
        << ", скорость=" << vehicle->speed
        << ", тип=" << vehicle->getType() << endl;

    auto violations = zone->check(vehicle);

    if (violations.empty()) {
        cout << "  [VideoProcessor] Нарушений не обнаружено" << endl;
        return;
    }

    for (auto violation : violations) {
        cout << "  [VideoProcessor] Обнаружено нарушение: " << violation->getDescription() << endl;

        Evidence* evidence = evidenceCollector->collect(violation, frames, frameQuality);

        if (evidence) {
            evidence->save();
            Resolution* resolution = resolutionGenerator->generate(evidence, vehicle->speed, ocrConfidence);
            if (resolution) {
                resolution->save();
                delete resolution;
            }
            delete evidence;
        }
        delete violation;
    }
}