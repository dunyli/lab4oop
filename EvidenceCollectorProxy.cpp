#include "EvidenceCollectorProxy.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

EvidenceCollectorProxy::EvidenceCollectorProxy(int q, int before, int after)
    : minQuality(q), framesBefore(before), framesAfter(after) {
    cout << "[EvidenceCollectorProxy] Создан: качество=" << q
        << ", кадров до=" << before << ", после=" << after << endl;
}

Evidence* EvidenceCollectorProxy::collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime) {
    cout << "  [EvidenceCollectorProxy] Качество: " << quality << " (нужно " << minQuality
        << "), Кадров: " << frames.size() << " (нужно " << framesBefore << " до, "
        << framesAfter << " после)" << endl;

    if (quality < minQuality) {
        cout << "  [EvidenceCollectorProxy] Качество слишком низкое - ОТКЛОНЕНО" << endl;
        return nullptr;
    }

    if (frames.size() < framesBefore + framesAfter + 1) {
        cout << "  [EvidenceCollectorProxy] Недостаточно кадров - ОТКЛОНЕНО" << endl;
        return nullptr;
    }

    cout << "  [EvidenceCollectorProxy] Проверка пройдена - вызов базового метода" << endl;
    return EvidenceCollector::collect(v, frames, quality, violationTime);
}