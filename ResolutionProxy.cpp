#include "ResolutionProxy.h"
#include "Resolution.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

ResolutionProxy::ResolutionProxy(bool requireMetadataCheck, int minConfidenceLevel, bool enableAutoRequest)
    : requireMetadata(requireMetadataCheck), minConfidence(minConfidenceLevel), autoRequest(enableAutoRequest) {
    cout << "[ResolutionProxy] Инициализирован защитный прокси: "
        << "проверка метаданных=" << (requireMetadataCheck ? "включена" : "выключена")
        << ", минимальная уверенность=" << minConfidenceLevel
        << ", автозапрос=" << (enableAutoRequest ? "включен" : "выключен") << endl;
}

Resolution* ResolutionProxy::generate(Evidence* evidence, int vehicleSpeed, int recognitionConfidence) {
    cout << "  [ResolutionProxy] Финальная верификация: доказательства="
        << (evidence ? "присутствуют" : "отсутствуют")
        << ", уверенность распознавания=" << recognitionConfidence
        << " (требуется >= " << minConfidence << ")" << endl;

    // Проверка наличия доказательств
    if (!evidence) {
        cout << "  [ResolutionProxy] ОТКЛОНЕНО: отсутствуют доказательства нарушения" << endl;
        if (autoRequest) {
            cout << "  [ResolutionProxy] Инициирован автозапрос недостающих материалов с соседних камер" << endl;
        }
        return nullptr;
    }

    // Проверка уверенности распознавания номерного знака
    if (recognitionConfidence < minConfidence) {
        cout << "  [ResolutionProxy] ОТКЛОНЕНО: недостаточная уверенность распознавания номера ("
            << recognitionConfidence << " < " << minConfidence << ")" << endl;
        return nullptr;
    }

    cout << "  [ResolutionProxy] ОДОБРЕНО: все проверки пройдены успешно" << endl;

    // Делегирование формирования постановления базовому генератору
    return ResolutionGenerator::generate(evidence, vehicleSpeed, recognitionConfidence);
}