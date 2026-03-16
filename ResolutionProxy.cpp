#include "ResolutionProxy.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// Конструктор с параметрами настройки
ResolutionProxy::ResolutionProxy(bool meta, int conf, bool autoReq)
    : requireMetadata(meta), minConfidence(conf), autoRequest(autoReq) {
    cout << "[ResolutionProxy] Создан с параметрами: метаданные="
         << (meta ? "да" : "нет") << ", уверенность=" << conf
         << ", автозапрос=" << (autoReq ? "да" : "нет") << endl;
}

// Метод генерации постановления с проверкой
Resolution* ResolutionProxy::generate(Evidence* e, int speed, int confidence) {
    // Вывод информации о проверке
    cout << "  [ResolutionProxy] Проверка: доказательства="
         << (e ? "есть" : "нет") << ", уверенность=" << confidence
         << " (нужно " << minConfidence << "), метаданные="
         << (requireMetadata ? "требуются" : "не требуются") << endl;

    // Проверка наличия доказательств
    if (!e) {
        cout << "  [ResolutionProxy] Нет доказательств — ОТКЛОНЕНО" << endl;
        if (autoRequest) {
            cout << "  [ResolutionProxy] Автозапрос недостающих данных..." << endl;
        }
        return nullptr;
    }

    // Проверка уверенности распознавания номера
    if (confidence < minConfidence) {
        cout << "  [ResolutionProxy] Уверенность слишком низкая — ОТКЛОНЕНО" << endl;
        return nullptr;
    }

    // Если все проверки пройдены - создаем постановление
    cout << "  [ResolutionProxy] Проверка пройдена — СОЗДАНИЕ постановления" << endl;

    // Расчет суммы штрафа в зависимости от скорости
    int amount = 500; // базовая сумма
    if (speed > 80) {
        amount = 1500;
    }
    else if (speed > 70) {
        amount = 1000;
    }

    // Генерация уникального ID и создание постановления
    string resId = "RES_" + to_string(time(nullptr));
    return new Resolution(resId, amount);
}