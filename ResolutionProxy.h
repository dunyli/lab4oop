#pragma once
#include "Resolution.h"
#include "Evidence.h"

using namespace std;

//  ласс-прокси дл€ финальной проверки перед созданием постановлени€
class ResolutionProxy {
public:
    bool requireMetadata;      // “ребовать ли проверку метаданных
    int minConfidence;         // ћинимальна€ уверенность распознавани€ номера (0-10)
    bool autoRequest;          // јвтоматически запрашивать недостающие данные

    //  онструктор с параметрами настройки
    ResolutionProxy(bool meta, int conf = 9, bool autoReq = false);

    // ћетод генерации постановлени€ с проверкой
    Resolution* generate(Evidence* e, int speed, int confidence = 10);
};