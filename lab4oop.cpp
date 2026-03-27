#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <queue>
#include <list>
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
#include "Violation.h"
#include "SpeedViolation.h"
#include "Evidence.h"
#include "Resolution.h"
#include "LaserSensor.h"
#include "LaserAdapter.h"
#include "RadarSensor.h"
#include "RadarAdapter.h"
#include "DescriptionViolation.h"
#include "PriorityViolation.h"
#include "SimpleFrameBuffer.h"
#include "CompositeFrameBuffer.h"
#include "FrameIterator.h"

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

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));

    cout << "СИСТЕМА ФИКСАЦИИ НАРУШЕНИЙ ПДД" << endl;
    cout << "ДЕМОНСТРАЦИЯ ТРЕХ КОНФИГУРАЦИЙ" << endl;

    // ========================================
    // КОНФИГУРАЦИЯ 1: БАЗОВАЯ (без паттернов)
    // ========================================
    cout << "КОНФИГУРАЦИЯ 1: БАЗОВАЯ" << endl;
    cout << "Минимальная конфигурация: только скорость, без прокси, без декораторов" << endl;
    cout << string(70, '=') << endl;

    // Создание зоны контроля с базовым правилом
    ControlZone* zone1 = new ControlZone();
    zone1->addRule(new SpeedRule(60));

    // Базовые компоненты без паттернов
    EvidenceCollector* collector1 = new EvidenceCollector();
    ResolutionGenerator* generator1 = new ResolutionGenerator();
    VideoProcessor* processor1 = new VideoProcessor(zone1, collector1, generator1);
    Camera* camera1 = new Camera();

    Vehicle* car1 = createTestCar("CAR_001", 75, 1, "PassengerCar");

    vector<Frame*> frames1;
    for (int i = 0; i < 3; i++) {
        frames1.push_back(camera1->getFrame());
    }

    processor1->processFrameWithFrames(car1, 9, 10, frames1);

    // Очистка конфигурации 1
    for (auto f : frames1) delete f;
    delete car1;
    delete processor1;
    delete generator1;
    delete collector1;
    delete camera1;
    delete zone1;

    // ========================================
    // КОНФИГУРАЦИЯ 2: СТАНДАРТНАЯ (Proxy + Adapter + Decorator)
    // ========================================
    cout << "КОНФИГУРАЦИЯ 2: СТАНДАРТНАЯ" << endl;
    cout << "Proxy + Adapter + Decorator: буферизация, проверка качества, адаптер лазера" << endl;
    cout << string(70, '=') << endl;

    // Зона контроля с полным набором правил
    ControlZone* zone2 = new ControlZone();
    zone2->addRule(new SpeedRule(60));
    zone2->addRule(new TrafficLightRule());
    zone2->addRule(new LineCrossRule());
    zone2->addRule(new BusLaneRule(1));

    // ДЕМОНСТРАЦИЯ ADAPTER: LaserAdapter
    cout << "\nAdapter: LaserAdapter" << endl;
    LaserSensor* laser = new LaserSensor();
    LaserAdapter* laserAdapter = new LaserAdapter(laser);
    cout << "  Лазерный измеритель: " << laser->getSpeedMph() << " миль/ч" << endl;
    cout << "  После адаптации (миль/ч - км/ч): " << laserAdapter->getSpeedKmh() << " км/ч" << endl;
    delete laserAdapter;
    delete laser;

    // ДЕМОНСТРАЦИЯ ADAPTER: RadarSensor
    cout << "\nAdapter: RadarSensor" << endl;
    RadarSensor* radar = new RadarSensor();
    RadarAdapter* radarAdapter = new RadarAdapter(radar);
    cout << "  Лазерный измеритель: " << radar->getSpeedMps() << " миль/ч" << endl;
    cout << "  После адаптации (м/с - км/ч): " << radarAdapter->getSpeedKmh() << " км/ч" << endl;
    delete radarAdapter;
    delete radar;

    // Proxy компоненты
    cout << "\nProxy компоненты" << endl;
    EvidenceCollector* collector2 = new EvidenceCollectorProxy(8, 2, 1);
    ResolutionGenerator* generator2 = new ResolutionProxy(true, 8, false);
    VideoProcessor* processor2 = new VideoProcessorProxy(zone2, collector2, generator2, 300);
    Camera* camera2 = new Camera();

    Vehicle* car2 = createTestCar("CAR_002", 82, 1, "PassengerCar");

    vector<Frame*> frames2;
    for (int i = 0; i < 10; i++) {
        frames2.push_back(camera2->getFrame());
    }
    // ДЕМОНСТРАЦИЯ DECORATOR: DescriptionViolation
    cout << "\nDecorator: DescriptionViolation" << endl;
    SpeedViolation* baseViolation = new SpeedViolation(car2, 82, 60);
    Violation* decoratedViolation = new DescriptionViolation(baseViolation, "камера 002");
    cout << "  Базовое нарушение: " << baseViolation->getDescription() << endl;
    cout << "  Декорированное (с комментарием): " << decoratedViolation->getDescription() << endl;
    delete decoratedViolation;

    // Обработка
    processor2->processFrameWithFrames(car2, 9, 10, frames2);

    // Очистка конфигурации 2
    for (auto f : frames2) delete f;
    delete car2;
    delete processor2;
    delete generator2;
    delete collector2;
    delete camera2;
    delete zone2;

    // ========================================
// КОНФИГУРАЦИЯ 3: МАКСИМАЛЬНАЯ (Composite + Adapter + Decorator + Iterator + Proxy)
// ========================================
    cout << "КОНФИГУРАЦИЯ 3: МАКСИМАЛЬНАЯ" << endl;
    cout << "Composite + Adapter + Decorator + Iterator + Proxy" << endl;
    cout << string(70, '=') << endl;

    // 1. COMPOSITE: CompositeFrameBuffer (объединение трех камер)
    cout << "\n 1. Composite: CompositeFrameBuffer" << endl;

    // Создаем три камеры
    Camera* camA = new Camera();
    Camera* camB = new Camera();
    Camera* camC = new Camera();

    // Создаем буферы для каждой камеры
    SimpleFrameBuffer* bufA = new SimpleFrameBuffer(300);
    SimpleFrameBuffer* bufB = new SimpleFrameBuffer(300);
    SimpleFrameBuffer* bufC = new SimpleFrameBuffer(300);

    // Заполняем буферы кадрами
    for (int i = 0; i < 50; i++) {
        Frame* fA = camA->getFrame();
        Frame* fB = camB->getFrame();
        Frame* fC = camC->getFrame();
        bufA->addFrame(fA);
        bufB->addFrame(fB);
        bufC->addFrame(fC);
    }

    // СОЗДАЕМ КОМПОЗИТНЫЙ БУФЕР (объединяет все три буфера)
    CompositeFrameBuffer* compositeBuffer = new CompositeFrameBuffer();
    compositeBuffer->addBuffer(bufA);
    compositeBuffer->addBuffer(bufB);
    compositeBuffer->addBuffer(bufC);

    cout << "  Камера A: " << bufA->getSize() << " кадров" << endl;
    cout << "  Камера B: " << bufB->getSize() << " кадров" << endl;
    cout << "  Камера C: " << bufC->getSize() << " кадров" << endl;
    cout << "  Композитный буфер (объединение): " << compositeBuffer->getSize() << " кадров" << endl;
    cout << "  Видеопроцессор работает с композитом как с единым буфером" << endl;

    // 2. ADAPTER: RadarAdapter и LaserAdapter
    cout << "\n2. Adapter: RadarAdapter и LaserAdapter" << endl;

    RadarSensor* radar2 = new RadarSensor();
    RadarAdapter* radarAdapter2 = new RadarAdapter(radar2);
    cout << "  Радарный датчик: " << radar2->getSpeedMps() << " м/с → "
        << radarAdapter2->getSpeedKmh() << " км/ч" << endl;

    LaserSensor* laser2 = new LaserSensor();
    LaserAdapter* laserAdapter2 = new LaserAdapter(laser2);
    cout << "  Лазерный измеритель: " << laser2->getSpeedMph() << " миль/ч → "
        << laserAdapter2->getSpeedKmh() << " км/ч" << endl;

    delete radarAdapter2;
    delete radar2;
    delete laserAdapter2;
    delete laser2;

    // ========================================
    // 3. Зона контроля с полным набором правил
    // ========================================
    ControlZone* zone3 = new ControlZone();
    zone3->addRule(new SpeedRule(60));
    zone3->addRule(new TrafficLightRule());
    zone3->addRule(new LineCrossRule());
    zone3->addRule(new BusLaneRule(1));

    // ========================================
    // 4. Proxy для доказательств и постановлений
    // ========================================
    cout << "\n3. Proxy: EvidenceCollectorProxy и ResolutionProxy ---" << endl;
    EvidenceCollector* collector3 = new EvidenceCollectorProxy(9, 3, 3);
    ResolutionGenerator* generator3 = new ResolutionProxy(true, 9, true);
    VideoProcessor* processor3 = new VideoProcessorProxy(zone3, collector3, generator3, 300);

    // ========================================
    // 5. DECORATOR: PriorityViolation и DescriptionViolation
    // ========================================
    cout << "\n4. Decorator: PriorityViolation и DescriptionViolation" << endl;

    Vehicle* car3 = createTestCar("CAR_003", 95, 1, "PassengerCar");
    SpeedViolation* baseViolation2 = new SpeedViolation(car3, 95, 60);

    // Комбинируем декораторы
    Violation* decoratedViolation2 = new PriorityViolation(
        new DescriptionViolation(baseViolation2, "камера 003, туман"));

    cout << "  Базовое нарушение: " << baseViolation2->getDescription() << endl;
    cout << "  Декорированное: " << decoratedViolation2->getDescription() << endl;
    delete decoratedViolation2;

    // ========================================
    // 6. Обработка нарушения с использованием композитного буфера
    // ========================================
    cout << "\n 5. Обработка нарушения с композитным буфером" << endl;
    vector<Frame*> allFrames = compositeBuffer->getFrames();
    cout << "  Получено " << allFrames.size() << " кадров из композитного буфера" << endl;
    processor3->processFrameWithFrames(car3, 9, 10, allFrames);

    // 7. ITERATOR: FrameIterator для обхода кадров
    cout << "\n6. Iterator: FrameIterator для обхода кадров в доказательствах" << endl;

    Evidence* evidence = new Evidence("DEMO_EVID");
    for (int i = 0; i < 5; i++) {
        evidence->addFrame(new Frame(1000 + i * 100));
    }

    cout << "  Доказательства: " << evidence->id << ", кадров: " << evidence->frames.size() << endl;
    cout << "  Обход кадров через итератор:" << endl;

    for (FrameIterator it = evidence->begin(); it != evidence->end(); ++it) {
        Frame* f = *it;
        cout << "    Кадр: время " << f->timestamp << endl;
    }

    // ОЧИСТКА КОНФИГУРАЦИИ 3
    delete evidence;
    delete car3;
    delete processor3;
    delete generator3;
    delete collector3;
    delete compositeBuffer;  // CompositeBuffer удаляет bufA, bufB, bufC
    delete camA;
    delete camB;
    delete camC;
    delete zone3;

    // ИТОГ ДЕМОНСТРАЦИИ
    cout << "ИТОГ ДЕМОНСТРАЦИИ" << endl;
    cout << string(70, '=') << endl;

    cout << "\nКонфигурация 1 (БАЗОВАЯ): без паттернов" << endl;
    cout << "  - Только контроль скорости, без буферизации, без проверки качества" << endl;

    cout << "\nКонфигурация 2 (СТАНДАРТНАЯ): Adapter + Decorator + Proxy" << endl;
    cout << "  - Adapter (RadarAdapter, LaserAdapter): радарный и лазерный датчики" << endl;
    cout << "  - Decorator (DescriptionViolation): добавление комментариев к нарушениям" << endl;
    cout << "  - Proxy (VideoProcessorProxy, EvidenceCollectorProxy, ResolutionProxy):" << endl;
    cout << "    буферизация кадров (300 кадров)" << endl;
    cout << "    проверка качества (порог 8, 2 кадра до, 1 после)" << endl;
    cout << "    финальная верификация постановлений (метаданные, уверенность 8)" << endl;

    cout << "\nКонфигурация 3 (МАКСИМАЛЬНАЯ): Composite + Adapter + Decorator + Iterator + Proxy" << endl;
    cout << "  - Composite (CompositeFrameBuffer): объединение буферов трех камер в один" << endl;
    cout << "  - Adapter (RadarAdapter, LaserAdapter): радарный и лазерный датчики" << endl;
    cout << "  - Decorator (PriorityViolation, DescriptionViolation): пометка 'СРОЧНО!' и комментарии" << endl;
    cout << "  - Iterator (FrameIterator): последовательный обход кадров в доказательствах" << endl;
    cout << "  - Proxy: строгая проверка качества (9, 3/3), автозапрос данных" << endl;

    cout << "ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА" << endl;

    return 0;
}