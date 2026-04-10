#include "EvidenceCollectorProxy.h"
#include "EvidenceCollector.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

EvidenceCollectorProxy::EvidenceCollectorProxy(int qualityThreshold, int framesBeforeCount, int framesAfterCount)
    : minQuality(qualityThreshold), framesBefore(framesBeforeCount), framesAfter(framesAfterCount) {
    cout << "[EvidenceCollectorProxy] Инициализирован защитный прокси: "
        << "минимальное качество=" << qualityThreshold
        << ", кадров до нарушения=" << framesBeforeCount
        << ", кадров после нарушения=" << framesAfterCount << endl;
}

Evidence* EvidenceCollectorProxy::collect(Violation* violation, vector<Frame*>& frames, int quality, long violationTime) {
    cout << "  [EvidenceCollectorProxy] Входная проверка: качество кадров=" << quality
        << " (требуется >= " << minQuality << "), "
        << "количество кадров=" << frames.size()
        << " (требуется минимум " << (framesBefore + framesAfter + 1) << ")" << endl;

    // Проверка качества изображения (четкость, освещенность)
    if (quality < minQuality) {
        cout << "  [EvidenceCollectorProxy] ОТКЛОНЕНО: качество кадров недостаточно ("
            << quality << " < " << minQuality << ")" << endl;
        return nullptr;
    }

    // Проверка полноты набора кадров (до и после нарушения)
    int requiredFrames = framesBefore + framesAfter + 1;
    if ((int)frames.size() < requiredFrames) {
        cout << "  [EvidenceCollectorProxy] ОТКЛОНЕНО: недостаточно кадров для доказательств ("
            << frames.size() << " < " << requiredFrames << ")" << endl;
        return nullptr;
    }

    cout << "  [EvidenceCollectorProxy] ОДОБРЕНО: все проверки качества пройдены" << endl;

    // Делегирование создания доказательств базовому сборщику
    return EvidenceCollector::collect(violation, frames, quality, violationTime);
}