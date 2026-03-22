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

using namespace std;

Vehicle* createTestCar(string id, int speed, int lane, string type = "PassengerCar") {
    Vehicle* v;
    if (type == "Bus") v = new Bus(id);
    else v = new PassengerCar(id);
    v->speed = speed;
    v->lane = lane;
    v->plate = new LicensePlate("A123BC");
    return v;
}

vector<Frame*> getAllFrames(queue<Frame*> buffer) {
    vector<Frame*> frames;
    queue<Frame*> temp = buffer;
    while (!temp.empty()) {
        frames.push_back(temp.front());
        temp.pop();
    }
    return frames;
}

void demonstrateConfig(string name, ControlZone* zone, EvidenceCollector* evidenceCollector,
    ResolutionGenerator* resolutionGenerator, int bufferSize, Vehicle* testCar, bool useProxy) {
    cout << "КОНФИГУРАЦИЯ: " << name << endl;

    cout << "\nКОМПОНЕНТЫ КОНФИГУРАЦИИ\n";

    VideoProcessor* processor = nullptr;
    VideoProcessorProxy* processorProxy = nullptr;

    if (useProxy && bufferSize > 0) {
        processorProxy = new VideoProcessorProxy(zone, evidenceCollector, resolutionGenerator, bufferSize);
        processor = processorProxy;
        cout << "Видеопроцессор: VideoProcessorProxy (буфер=" << bufferSize << ")\n";
    }
    else {
        processor = new VideoProcessor(zone, evidenceCollector, resolutionGenerator);
        cout << "Видеопроцессор: VideoProcessor\n";
    }

    EvidenceCollectorProxy* ecProxy = dynamic_cast<EvidenceCollectorProxy*>(evidenceCollector);
    if (ecProxy) {
        cout << "Сборщик: EvidenceCollectorProxy (качество>=" << ecProxy->getMinQuality()
            << ", кадров до=" << ecProxy->getFramesBefore() << ", после=" << ecProxy->getFramesAfter() << ")\n";
    }
    else {
        cout << "Сборщик: EvidenceCollector (без проверок)\n";
    }

    ResolutionProxy* rpProxy = dynamic_cast<ResolutionProxy*>(resolutionGenerator);
    if (rpProxy) {
        cout << "Генератор: ResolutionProxy (метаданные=" << (rpProxy->getRequireMetadata() ? "да" : "нет")
            << ", уверенность>=" << rpProxy->getMinConfidence() << ", автозапрос=" << (rpProxy->getAutoRequest() ? "да" : "нет") << ")\n";
    }
    else {
        cout << "Генератор: ResolutionGenerator (без проверок)\n";
    }

    Camera* camera = new Camera();

    if (useProxy && bufferSize > 0 && processorProxy) {
        cout << "\nЗаполнение буфера (" << bufferSize << " кадров) \n";
        for (int i = 0; i < bufferSize; i++) {
            Frame* f = camera->getFrame();
            processorProxy->addToBuffer(f);
        }
        cout << "Буфер заполнен, размер: " << processorProxy->getBufferSize() << " кадров\n";
    }

    cout << "\nТЕСТИРОВАНИЕ НАРУШЕНИЙ\n";

    vector<Frame*> allFrames;
    if (processorProxy) {
        allFrames = processorProxy->getAllFrames();
        cout << "Получено " << allFrames.size() << " кадров из буфера\n";
    }

    // Тест 1
    testCar->speed = 95;
    cout << "\n ТЕСТ 1: Превышение скорости (" << testCar->speed << " км/ч) - высокое качество\n";
    processor->processFrameWithFrames(testCar, 9, 10, allFrames);

    // Тест 2
    testCar->speed = 82;
    cout << "\n ТЕСТ 2: Превышение скорости (" << testCar->speed << " км/ч) - низкое качество\n";
    processor->processFrameWithFrames(testCar, 3, 10, allFrames);

    // Тест 3
    Vehicle* bus = new Bus("BUS_001");
    bus->speed = 55;
    bus->lane = 1;
    bus->plate = new LicensePlate("B456CM");
    cout << "\nТЕСТ 3: Автобус на выделенной полосе - не должно быть нарушения\n";
    processor->processFrameWithFrames(bus, 9, 10, allFrames);

    delete bus;
    delete processor;
    delete camera;
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));
    cout << "СИСТЕМА ФИКСАЦИИ НАРУШЕНИЙ ПДД" << endl;

    cout << "\nОБЩАЯ СХЕМА РАБОТЫ ПРОКСИ\n";
    cout << "1. VideoProcessorProxy (Фильтрующий) - наследник VideoProcessor, буферизация кадров\n";
    cout << "2. EvidenceCollectorProxy (Защитный) - наследник EvidenceCollector, проверка качества\n";
    cout << "3. ResolutionProxy (Защитный) - наследник ResolutionGenerator, финальная проверка\n";

    // Конфигурация 1
    ControlZone* zone1 = new ControlZone();
    zone1->addRule(new SpeedRule(60));
    zone1->addRule(new TrafficLightRule());
    zone1->addRule(new LineCrossRule());
    zone1->addRule(new BusLaneRule(1));

    EvidenceCollector* evidence1 = new EvidenceCollectorProxy(9, 3, 3);
    ResolutionGenerator* resolution1 = new ResolutionProxy(true, 9, false);
    Vehicle* testCar1 = createTestCar("CAR_001", 75, 1, "PassengerCar");

    demonstrateConfig("ПЕРЕКРЕСТОК В ЦЕНТРЕ ГОРОДА (строгие требования)",
        zone1, evidence1, resolution1, 300, testCar1, true);

    delete testCar1;

    // Конфигурация 2
    ControlZone* zone2 = new ControlZone();
    zone2->addRule(new SpeedRule(90));

    EvidenceCollector* evidence2 = new EvidenceCollectorProxy(7, 2, 1);
    ResolutionGenerator* resolution2 = new ResolutionProxy(false, 7, false);
    Vehicle* testCar2 = createTestCar("CAR_002", 105, 1, "PassengerCar");

    demonstrateConfig("ЗАГОРОДНАЯ ТРАССА (сниженные требования)",
        zone2, evidence2, resolution2, 450, testCar2, true);

    delete testCar2;

    // Конфигурация 3
    ControlZone* zone3 = new ControlZone();
    zone3->addRule(new SpeedRule(60));
    zone3->addRule(new BusLaneRule(1));

    EvidenceCollector* evidence3 = new EvidenceCollectorProxy(8, 2, 1);
    ResolutionGenerator* resolution3 = new ResolutionProxy(true, 8, true);
    Vehicle* testCar3 = createTestCar("CAR_003", 70, 1, "PassengerCar");

    demonstrateConfig("ВЫДЕЛЕННАЯ ПОЛОСА В ЧАСЫ ПИК (средние требования)",
        zone3, evidence3, resolution3, 150, testCar3, true);

    delete testCar3;

    // Конфигурация 4 (без прокси)
    ControlZone* zone4 = new ControlZone();
    zone4->addRule(new SpeedRule(60));

    EvidenceCollector* evidence4 = new EvidenceCollector();
    ResolutionGenerator* resolution4 = new ResolutionGenerator();
    Vehicle* testCar4 = createTestCar("CAR_004", 85, 1, "PassengerCar");

    demonstrateConfig("МИНИМАЛЬНАЯ НАГРУЗКА (без прокси)",
        zone4, evidence4, resolution4, 0, testCar4, false);

    delete testCar4;

    cout << "\n" << string(70, '=') << endl;
    cout << "ИТОГ ДЕМОНСТРАЦИИ:" << endl;
    cout << "VideoProcessorProxy (наследник VideoProcessor) - буферизация кадров\n";
    cout << "EvidenceCollectorProxy (наследник EvidenceCollector) - проверка качества\n";
    cout << "ResolutionProxy (наследник ResolutionGenerator) - финальная проверка\n";
    cout << "ДЕЛЕГИРОВАНИЕ: VideoProcessor - ControlZone - Rule\n";
    cout << string(70, '=') << endl;

    delete zone1; delete evidence1; delete resolution1;
    delete zone2; delete evidence2; delete resolution2;
    delete zone3; delete evidence3; delete resolution3;
    delete zone4; delete evidence4; delete resolution4;

    cout << "\nРАБОТА ЗАВЕРШЕНА\n";
    return 0;
}