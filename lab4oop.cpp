#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <queue>
#include <locale>
#include "Camera.h"
#include "VideoProcessor.h"
#include "VideoProcessorProxy.h"
#include "ControlZone.h"
#include "SpeedRule.h"
#include "TrafficLightRule.h"
#include "LineCrossRule.h"
#include "BusLaneRule.h"
#include "EvidenceCollectorProxy.h"
#include "ResolutionProxy.h"
#include "PassengerCar.h"
#include "Bus.h"

using namespace std;

// Вспомогательная функция для создания тестового ТС
Vehicle* createTestCar(string id, int speed, int lane, string type = "PassengerCar") {
    Vehicle* v;
    if (type == "Bus") v = new Bus(id);
    else v = new PassengerCar(id);

    v->speed = speed;
    v->lane = lane;
    v->plate = new LicensePlate("A123BC");
    return v;
}

// Функция для получения всех кадров из буфера
vector<Frame*> getAllFrames(queue<Frame*> buffer) {
    vector<Frame*> frames;
    queue<Frame*> temp = buffer;
    while (!temp.empty()) {
        frames.push_back(temp.front());
        temp.pop();
    }
    return frames;
}

// Функция для демонстрации конфигурации
void demonstrateConfig(string name,
    ControlZone* zone,
    EvidenceCollectorProxy* evidenceProxy,
    ResolutionProxy* resolutionProxy,
    int bufferSize,
    Vehicle* testCar) {

    cout << "\nКОНФИГУРАЦИЯ: " << name << endl;

    // Пояснение по прокси
    cout << "\n--- РАБОТА ПРОКСИ В ДАННОЙ КОНФИГУРАЦИИ ---\n";
    cout << "VideoProcessorProxy: буферизация кадров (размер буфера: " << bufferSize << ")\n";
    cout << "  Отвечает за сохранение истории кадров для последующего доступа\n";
    cout << "EvidenceCollectorProxy: контроль качества доказательств\n";
    cout << "  Проверяет: качество >= " << evidenceProxy->minQuality << "/10, кадров до: "
        << evidenceProxy->framesBefore << ", после: " << evidenceProxy->framesAfter << "\n";
    cout << "ResolutionProxy: финальная проверка перед постановлением\n";
    cout << "  Проверяет: метаданные: " << (resolutionProxy->requireMetadata ? "да" : "нет")
        << ", уверенность >= " << resolutionProxy->minConfidence << "/10, автозапрос: "
        << (resolutionProxy->autoRequest ? "да" : "нет") << "\n";

    // Создаем процессор и прокси
    VideoProcessor* processor = new VideoProcessor(zone, evidenceProxy, resolutionProxy);
    VideoProcessorProxy* processorProxy = new VideoProcessorProxy(processor, bufferSize);
    Camera* camera = new Camera();

    cout << "\nЗаполнение буфера\n";
    for (int i = 0; i < bufferSize; i++) {
        Frame* f = camera->getFrame();
        processorProxy->buffer.push(f);
    }
    cout << "Буфер заполнен, размер: " << processorProxy->buffer.size() << " кадров\n";

    // Тестируем нарушения с передачей кадров из буфера
    cout << "\nТЕСТИРОВАНИЕ НАРУШЕНИЙ\n";

    // Получаем кадры из буфера
    vector<Frame*> allFrames = getAllFrames(processorProxy->buffer);
    cout << "Получено " << allFrames.size() << " кадров из буфера для анализа\n";

    // Тест 1: Превышение скорости (должно пройти)
    testCar->speed = 95;
    cout << "\n ТЕСТ 1: Превышение скорости (" << testCar->speed << " км/ч) - высокое качество, достаточно кадров\n";
    cout << "   [VideoProcessorProxy] Передает запрос к буферу\n";
    processor->processFrameWithFrames(testCar, 9, 10, allFrames); // качество 9, уверенность 10

    // Тест 2: Превышение скорости (низкое качество)
    testCar->speed = 82;
    cout << "\n ТЕСТ 2: Превышение скорости (" << testCar->speed << " км/ч) - низкое качество кадра\n";
    cout << "   [VideoProcessorProxy] Передает запрос к буферу\n";
    processor->processFrameWithFrames(testCar, 3, 10, allFrames); // качество 3

    // Тест 3: Автобус на выделенной полосе (не нарушение)
    Vehicle* bus = new Bus("BUS_001");
    bus->speed = 55;
    bus->lane = 1;
    bus->plate = new LicensePlate("B456CM");

    cout << "\nТЕСТ 3: Автобус на выделенной полосе (полоса 1) - не должно быть нарушения\n";
    cout << "   [VideoProcessorProxy] Передает запрос к буферу\n";
    processor->processFrameWithFrames(bus, 9, 10, allFrames);

    delete bus;

    // Очистка
    delete processorProxy;
    delete processor;
    delete camera;
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));

    cout << "СИСТЕМА ФИКСАЦИИ НАРУШЕНИЙ ПДД" << endl;
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ В ТРЕХ КОНФИГУРАЦИЯХ" << endl;

    cout << "\n--- ОБЩАЯ СХЕМА РАБОТЫ ПРОКСИ ---\n";
    cout << "1. VideoProcessorProxy (Фильтрующий прокси):\n";
    cout << "Перехватывает все кадры от камеры\n";
    cout << "Сохраняет их в буфер (кольцевой буфер)\n";
    cout << "При запросе возвращает кадры за последние N секунд\n";
    cout << " Позволяет получить кадры ДО момента нарушения\n\n";

    cout << "2. EvidenceCollectorProxy (Защитный прокси):\n";
    cout << "Проверяет качество каждого кадра (четкость, освещенность)\n";
    cout << "Проверяет наличие кадров до и после нарушения\n";
    cout << "Отсеивает бракованные материалы\n\n";

    cout << "3. ResolutionProxy (Защитный прокси):\n";
    cout << " Проверяет полноту доказательств\n";
    cout << " Проверяет читаемость номерного знака\n";
    cout << " Проверяет метаданные и временные метки\n";
    cout << " Финальная проверка перед выпиской штрафа\n\n";

    // КОНФИГУРАЦИЯ 1: Перекресток в центре города
    ControlZone* zone1 = new ControlZone();
    zone1->addRule(new SpeedRule(60));      // лимит 60
    zone1->addRule(new TrafficLightRule()); // контроль светофора
    zone1->addRule(new LineCrossRule());    // контроль стоп-линии
    zone1->addRule(new BusLaneRule(1));     // выделенная полоса

    EvidenceCollectorProxy* evidenceProxy1 = new EvidenceCollectorProxy(9, 3, 3);  // качество 9, 3 кадра до, 3 после
    ResolutionProxy* resolutionProxy1 = new ResolutionProxy(true, 9, false);      // метаданные да, уверенность 9, без автозапроса

    Vehicle* testCar1 = createTestCar("CAR_001", 75, 1, "PassengerCar");

    demonstrateConfig("ПЕРЕКРЕСТОК В ЦЕНТРЕ ГОРОДА\nСтрогие требования",
        zone1, evidenceProxy1, resolutionProxy1, 300, testCar1);

    delete testCar1;

    // КОНФИГУРАЦИЯ 2: Загородная трасса
    ControlZone* zone2 = new ControlZone();
    zone2->addRule(new SpeedRule(90));      // только скорость, лимит 90

    EvidenceCollectorProxy* evidenceProxy2 = new EvidenceCollectorProxy(7, 2, 1);  // качество 7, 2 кадра до, 1 после
    ResolutionProxy* resolutionProxy2 = new ResolutionProxy(false, 7, false);     // метаданные нет, уверенность 7

    Vehicle* testCar2 = createTestCar("CAR_002", 105, 1, "PassengerCar");

    demonstrateConfig("ЗАГОРОДНАЯ ТРАССА\nСниженные требования",
        zone2, evidenceProxy2, resolutionProxy2, 450, testCar2);

    delete testCar2;
    // КОНФИГУРАЦИЯ 3: Выделенная полоса в часы пик
    ControlZone* zone3 = new ControlZone();
    zone3->addRule(new SpeedRule(60));      // скорость 60
    zone3->addRule(new BusLaneRule(1));     // выделенная полоса

    EvidenceCollectorProxy* evidenceProxy3 = new EvidenceCollectorProxy(8, 2, 1);  // качество 8, 2 кадра до, 1 после
    ResolutionProxy* resolutionProxy3 = new ResolutionProxy(true, 8, true);       // метаданные да, уверенность 8, с автозапросом

    Vehicle* testCar3 = createTestCar("CAR_003", 70, 1, "PassengerCar");

    demonstrateConfig("ВЫДЕЛЕННАЯ ПОЛОСА В ЧАСЫ ПИК\nСредние требования",
        zone3, evidenceProxy3, resolutionProxy3, 150, testCar3);

    delete testCar3;

    // ИТОГ
    cout << "ИТОГ ДЕМОНСТРАЦИИ:\n";

    cout << "\nVideoProcessorProxy успешно буферизирует кадры и предоставляет\n";
    cout << "  доступ к истории для анализа нарушений\n";

    cout << "\nEvidenceCollectorProxy эффективно отсеивает некачественные материалы:\n";
    cout << "В конфигурации 1 (перекресток) - строгий отбор (качество 9)\n";
    cout << "В конфигурации 2 (трасса) - сниженные требования (качество 7)\n";
    cout << "В конфигурации 3 (полоса) - средние требования (качество 8)\n";

    cout << "\nResolutionProxy выполняет финальную проверку:\n";
    cout << "Проверяет наличие доказательств\n";
    cout << "Проверяет уверенность распознавания номера\n";
    cout << "Рассчитывает сумму штрафа\n";

    cout << "\nДЕЛЕГИРОВАНИЕ: VideoProcessor делегирует проверку правилам,\n";
    cout << "  а сбор доказательств и формирование постановления - прокси\n";

    // Очистка
    delete zone1;
    delete evidenceProxy1;
    delete resolutionProxy1;

    delete zone2;
    delete evidenceProxy2;
    delete resolutionProxy2;

    delete zone3;
    delete evidenceProxy3;
    delete resolutionProxy3;

    cout << "\nРАБОТА ЗАВЕРШЕНА\n";

    return 0;
}