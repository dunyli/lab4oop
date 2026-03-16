#include "EvidenceCollectorProxy.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// Конструктор с параметрами настройки
EvidenceCollectorProxy::EvidenceCollectorProxy(int q, int before, int after)
    : minQuality(q), framesBefore(before), framesAfter(after) {
    cout << "[EvidenceCollectorProxy] Создан с параметрами: качество=" << q
        << ", кадров до=" << before << ", после=" << after << endl;
}

// Метод сбора доказательств с проверкой качества и количества кадров
Evidence* EvidenceCollectorProxy::collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime) {
    // Вывод информации о проверке
    cout << "  [EvidenceCollectorProxy] Качество: " << quality << " (нужно " << minQuality
        << "), Кадров: " << frames.size() << " (нужно " << framesBefore << " до, "
        << framesAfter << " после)" << endl;

    // Проверка качества кадра
    if (quality < minQuality) {
        cout << "  [EvidenceCollectorProxy] Качество слишком низкое - ОТКЛОНЕНО" << endl;
        return nullptr;
    }

    // Проверка достаточного количества кадров (до и после нарушения)
    if (frames.size() < framesBefore + framesAfter + 1) {
        cout << "  [EvidenceCollectorProxy] Недостаточно кадров - ОТКЛОНЕНО" << endl;
        return nullptr;
    }

    // Если все проверки пройдены - создаем доказательства
    cout << "  [EvidenceCollectorProxy] Проверка пройдена - СОЗДАНИЕ доказательств" << endl;

    // Генерируем уникальный ID для доказательств
    string evidId = "EVID_" + to_string(time(nullptr));
    Evidence* e = new Evidence(evidId);

    // Добавляем все кадры в доказательства
    for (auto f : frames) {
        e->addFrame(f);
    }

    return e;
}