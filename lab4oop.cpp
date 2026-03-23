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
#include "EvidenceCollector.h"
#include "EvidenceCollectorProxy.h"
#include "ResolutionGenerator.h"
#include "ResolutionProxy.h"
#include "PassengerCar.h"
#include "Bus.h"
#include "LicensePlate.h"
#include "Frame.h"

using namespace std;

// Вспомогательная функция для создания тестового транспортного средства
Vehicle* createTestCar(string id, int speed, int lane, string type = "PassengerCar") {
    Vehicle* v;
    if (type == "Bus") {
        v = new Bus(id);
    }
    else {
        v = new PassengerCar(id);
    }
    v->speed = speed;
    v->lane = lane;
    v->plate = new LicensePlate("A123BC", "122");
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

// Функция для демонстрации работы конфигурации
void demonstrateConfig(string name, ControlZone* zone, EvidenceCollector* evidenceCollector,
    ResolutionGenerator* resolutionGenerator, int bufferSize,
    Vehicle* testCar, bool useProxy) {

    cout << "\n" << string(70, '=') << endl;
    cout << "КОНФИГУРАЦИЯ: " << name << endl;
    cout << string(70, '=') << endl;

    cout << "\n--- КОМПОНЕНТЫ КОНФИГУРАЦИИ ---\n";

    VideoProcessor* processor = nullptr;
    VideoProcessorProxy* processorProxy = nullptr;

    // Создание видеопроцессора (базовый или прокси)
    if (useProxy && bufferSize > 0) {
        processorProxy = new VideoProcessorProxy(zone, evidenceCollector, resolutionGenerator, bufferSize);
        processor = processorProxy;
        cout << "Видеопроцессор: VideoProcessorProxy (с буферизацией, буфер=" << bufferSize << " кадров)\n";
    }
    else {
        processor = new VideoProcessor(zone, evidenceCollector, resolutionGenerator);
        cout << " Видеопроцессор: VideoProcessor (без буферизации)\n";
    }

    // Определение типа сборщика доказательств
    EvidenceCollectorProxy* ecProxy = dynamic_cast<EvidenceCollectorProxy*>(evidenceCollector);
    if (ecProxy != nullptr) {
        cout << " Сборщик доказательств: EvidenceCollectorProxy\n";
        cout << "  - Минимальное качество: " << ecProxy->getMinQuality() << "/10\n";
        cout << "  - Кадров до нарушения: " << ecProxy->getFramesBefore() << "\n";
        cout << "  - Кадров после нарушения: " << ecProxy->getFramesAfter() << "\n";
    }
    else {
        cout << "Сборщик доказательств: EvidenceCollector (без проверок качества)\n";
    }

    // Определение типа генератора постановлений
    ResolutionProxy* rpProxy = dynamic_cast<ResolutionProxy*>(resolutionGenerator);
    if (rpProxy != nullptr) {
        cout << "Генератор постановлений: ResolutionProxy\n";
        cout << "  - Требование метаданных: " << (rpProxy->getRequireMetadata() ? "да" : "нет") << "\n";
        cout << "  - Минимальная уверенность номера: " << rpProxy->getMinConfidence() << "/10\n";
        cout << "  - Автозапрос недостающих данных: " << (rpProxy->getAutoRequest() ? "да" : "нет") << "\n";
    }
    else {
        cout << "Генератор постановлений: ResolutionGenerator (без дополнительных проверок)\n";
    }

    Camera* camera = new Camera();

    // Заполнение буфера кадрами (если используется прокси)
    if (useProxy && bufferSize > 0 && processorProxy != nullptr) {
        cout << "\nЗАПОЛНЕНИЕ БУФЕРА\n";
        for (int i = 0; i < bufferSize; i++) {
            Frame* f = camera->getFrame();
            processorProxy->addToBuffer(f);
        }
        cout << "Буфер заполнен, текущий размер: " << processorProxy->getBufferSize() << " / " << bufferSize << " кадров\n";
    }

    cout << "\nТЕСТИРОВАНИЕ НАРУШЕНИЙ\n";

    // Получение всех кадров из буфера для анализа
    vector<Frame*> allFrames;
    if (processorProxy != nullptr) {
        allFrames = processorProxy->getAllFrames();
        cout << "Получено " << allFrames.size() << " кадров из буфера для анализа\n";
    }

    // ТЕСТ 1: Превышение скорости с высоким качеством кадра
    testCar->speed = 95;
    cout << "\n ТЕСТ 1: Превышение скорости (" << testCar->speed << " км/ч) - высокое качество кадра (9/10)\n";
    processor->processFrameWithFrames(testCar, 9, 10, allFrames);

    // ТЕСТ 2: Превышение скорости с низким качеством кадра
    testCar->speed = 82;
    cout << "\n ТЕСТ 2: Превышение скорости (" << testCar->speed << " км/ч) - низкое качество кадра (3/10)\n";
    processor->processFrameWithFrames(testCar, 3, 10, allFrames);

    // ТЕСТ 3: Автобус на выделенной полосе (не должен фиксироваться как нарушитель)
    Vehicle* bus = new Bus("BUS_001");
    bus->speed = 55;
    bus->lane = 1;
    bus->plate = new LicensePlate("B456CM", "154");
    cout << "\n▶ ТЕСТ 3: Автобус на выделенной полосе (полоса 1) - не должно быть нарушения\n";
    processor->processFrameWithFrames(bus, 9, 10, allFrames);

    // Очистка памяти
    delete bus;
    delete processor;
    delete camera;
}

int main() {
    // Настройка локали для вывода русских символов
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));

    cout << "============================================================" << endl;
    cout << "СИСТЕМА ФИКСАЦИИ НАРУШЕНИЙ ПДД" << endl;
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ ПРОКСИ В ТРЕХ КОНФИГУРАЦИЯХ" << endl;
    cout << "============================================================" << endl;

    cout << "\n--- ОБЩАЯ СХЕМА РАБОТЫ ПРОКСИ ---\n";
    cout << "1. VideoProcessorProxy (Фильтрующий прокси) - наследник VideoProcessor\n";
    cout << "Добавляет буферизацию кадров\n";
    cout << "Позволяет получить кадры ДО момента нарушения\n\n";

    cout << "2. EvidenceCollectorProxy (Защитный прокси) - наследник EvidenceCollector\n";
    cout << "Проверяет качество кадров (четкость, освещенность)\n";
    cout << "Проверяет наличие кадров до и после нарушения\n";
    cout << "Отсеивает бракованные материалы\n\n";

    cout << "3. ResolutionProxy (Защитный прокси) - наследник ResolutionGenerator\n";
    cout << "Проверяет полноту доказательств\n";
    cout << "Проверяет уверенность распознавания номера\n";
    cout << "Выполняет финальную проверку перед выпиской штрафа\n";

    // ========================================
    // КОНФИГУРАЦИЯ 1: Перекресток в центре города (строгие требования)
    // ========================================
    ControlZone* zone1 = new ControlZone();
    zone1->addRule(new SpeedRule(60));      // контроль скорости
    zone1->addRule(new TrafficLightRule()); // контроль светофора
    zone1->addRule(new LineCrossRule());    // контроль стоп-линии
    zone1->addRule(new BusLaneRule(1));     // контроль выделенной полосы

    // переменные имеют тип БАЗОВОГО КЛАССА
    EvidenceCollector* evidence1 = new EvidenceCollectorProxy(9, 3, 3);      // качество 9, 3 до, 3 после
    ResolutionGenerator* resolution1 = new ResolutionProxy(true, 9, false);  // метаданные да, уверенность 9, автозапрос нет

    Vehicle* testCar1 = createTestCar("CAR_001", 75, 1, "PassengerCar");

    demonstrateConfig("ПЕРЕКРЕСТОК В ЦЕНТРЕ ГОРОДА (строгие требования)",
        zone1, evidence1, resolution1, 300, testCar1, true);

    delete testCar1;

    // ========================================
    // КОНФИГУРАЦИЯ 2: Загородная трасса (сниженные требования)
    // ========================================
    ControlZone* zone2 = new ControlZone();
    zone2->addRule(new SpeedRule(90));      // только контроль скорости

    EvidenceCollector* evidence2 = new EvidenceCollectorProxy(7, 2, 1);   // качество 7, 2 до, 1 после
    ResolutionGenerator* resolution2 = new ResolutionProxy(false, 7, false); // метаданные нет, уверенность 7

    Vehicle* testCar2 = createTestCar("CAR_002", 105, 1, "PassengerCar");

    demonstrateConfig("ЗАГОРОДНАЯ ТРАССА (сниженные требования)",
        zone2, evidence2, resolution2, 450, testCar2, true);

    delete testCar2;

    // ========================================
    // КОНФИГУРАЦИЯ 3: Выделенная полоса в часы пик (средние требования)
    // ========================================
    ControlZone* zone3 = new ControlZone();
    zone3->addRule(new SpeedRule(60));      // контроль скорости
    zone3->addRule(new BusLaneRule(1));     // контроль выделенной полосы

    EvidenceCollector* evidence3 = new EvidenceCollectorProxy(8, 2, 1);   // качество 8, 2 до, 1 после
    ResolutionGenerator* resolution3 = new ResolutionProxy(true, 8, true); // метаданные да, уверенность 8, автозапрос да

    Vehicle* testCar3 = createTestCar("CAR_003", 70, 1, "PassengerCar");

    demonstrateConfig("ВЫДЕЛЕННАЯ ПОЛОСА В ЧАСЫ ПИК (средние требования)",
        zone3, evidence3, resolution3, 150, testCar3, true);

    delete testCar3;

    // ========================================
    // КОНФИГУРАЦИЯ 4: Минимальная нагрузка (без прокси)
    // ========================================
    ControlZone* zone4 = new ControlZone();
    zone4->addRule(new SpeedRule(60));      // только контроль скорости

    // Используем БАЗОВЫЕ КЛАССЫ без прокси
    EvidenceCollector* evidence4 = new EvidenceCollector();           // без проверок качества
    ResolutionGenerator* resolution4 = new ResolutionGenerator();     // без финальной проверки

    Vehicle* testCar4 = createTestCar("CAR_004", 85, 1, "PassengerCar");

    demonstrateConfig("МИНИМАЛЬНАЯ НАГРУЗКА (без прокси)",
        zone4, evidence4, resolution4, 0, testCar4, false);

    delete testCar4;

    // Очистка памяти
    delete zone1;
    delete evidence1;
    delete resolution1;

    delete zone2;
    delete evidence2;
    delete resolution2;

    delete zone3;
    delete evidence3;
    delete resolution3;

    delete zone4;
    delete evidence4;
    delete resolution4;

    cout << "\nРАБОТА ЗАВЕРШЕНА" << endl;

    return 0;
}