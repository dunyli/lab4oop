#include "ResolutionProxy.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

ResolutionProxy::ResolutionProxy(bool meta, int conf, bool autoReq)
    : requireMetadata(meta), minConfidence(conf), autoRequest(autoReq) {
    cout << "[ResolutionProxy] Создан: метаданные=" << (meta ? "да" : "нет")
        << ", уверенность=" << conf << ", автозапрос=" << (autoReq ? "да" : "нет") << endl;
}

Resolution* ResolutionProxy::generate(Evidence* e, int speed, int confidence) {
    cout << "  [ResolutionProxy] Проверка: доказательства=" << (e ? "есть" : "нет")
        << ", уверенность=" << confidence << " (нужно " << minConfidence << ")" << endl;

    if (!e) {
        cout << "  [ResolutionProxy] Нет доказательств — ОТКЛОНЕНО" << endl;
        if (autoRequest) {
            cout << "  [ResolutionProxy] Автозапрос недостающих данных..." << endl;
        }
        return nullptr;
    }

    if (confidence < minConfidence) {
        cout << "  [ResolutionProxy] Уверенность слишком низкая — ОТКЛОНЕНО" << endl;
        return nullptr;
    }

    cout << "  [ResolutionProxy] Проверка пройдена — вызов базового метода" << endl;
    return ResolutionGenerator::generate(e, speed, confidence);
}