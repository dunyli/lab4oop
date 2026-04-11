#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

// Подключение всех компонентов системы
#include "Frame.h"
#include "Camera.h"
#include "CameraPool.h"
#include "LicensePlate.h"
#include "PassengerCar.h"
#include "Bus.h"
#include "Vehicle.h"
#include "Violation.h"
#include "Rule.h"
#include "SpeedRule.h"
#include "BusLaneRule.h"
#include "TrafficLightRule.h"
#include "LineCrossRule.h"
#include "ControlZone.h"
#include "Evidence.h"
#include "EvidenceCollector.h"
#include "EvidenceCollectorProxy.h"
#include "Resolution.h"
#include "ResolutionGenerator.h"
#include "ResolutionProxy.h"
#include "FineRegistry.h"
#include "SystemClock.h"
#include "RuleCreator.h"
#include "SpeedRuleCreator.h"
#include "BusLaneRuleCreator.h"
#include "AbstractZoneFactory.h"
#include "StrictCityFactory.h"
#include "HighwayFactory.h"
#include "ClonableViolation.h"
#include "SpeedViolationWithClone.h"
#include "VideoProcessor.h"

// Функция для вывода разделителя
void printSeparator(const string& title) {
    cout << "\n" << string(70, '=') << endl;
    cout << title << endl;
    cout << string(70, '=') << endl;
}

// ============================================================================
// СЦЕНАРИЙ 1: Базовая конфигурация
// ============================================================================
void demoScenario1() {
    printSeparator("СЦЕНАРИЙ 1: Базовая конфигурация (Singleton + Factory Method + Object Pool)");

    FineRegistry* registry = FineRegistry::getInstance();

    // 1. СОЗДАЁМ ПУЛ КАМЕР И ВРУЧНУЮ ДОБАВЛЯЕМ КАМЕРЫ
    CameraPool* camPool = CameraPool::getInstance(3);

    cout << "\n--- РУЧНОЕ ДОБАВЛЕНИЕ КАМЕР В ПУЛ ---" << endl;
    camPool->addCamera(new Camera());  // камера №1
    camPool->addCamera(new Camera());  // камера №2
    camPool->addCamera(new Camera());  // камера №3

    // 2. Factory Method: создаём правила через создателей
    RuleCreator* speedCreator60 = new SpeedRuleCreator(60);
    RuleCreator* speedCreator90 = new SpeedRuleCreator(90);
    RuleCreator* busCreator = new BusLaneRuleCreator(5);

    Rule* speedRule60 = speedCreator60->createRule();
    Rule* speedRule90 = speedCreator90->createRule();
    Rule* busRule = busCreator->createRule();

    // 3. Создаём зону контроля
    ControlZone* zone = new ControlZone(1, false);
    zone->addRule(speedRule60);
    zone->addRule(speedRule90);
    zone->addRule(busRule);

    // 4. Получаем камеры из пула и делаем снимки
    cout << "\n--- ПОЛУЧЕНИЕ КАМЕР ИЗ ПУЛА ---" << endl;
    vector<Camera*> cameras;
    vector<Frame*> frames;

    for (int i = 0; i < 3; i++) {
        Camera* cam = camPool->acquireCamera();
        if (cam != nullptr) {
            cameras.push_back(cam);
            Frame* frame = cam->getFrame();
            frames.push_back(frame);
            cout << "  Кадр " << i << " получен, timestamp=" << frame->timestamp << endl;
        }
    }

    // 5. Проверка автомобиля на нарушения
    cout << "\n--- ПРОВЕРКА АВТОМОБИЛЯ ---" << endl;
    PassengerCar* car = new PassengerCar("CAR_001");
    car->speed = 75;
    car->lane = 1;
    car->plate = new LicensePlate("A123BC", "77");

    auto violations = zone->check(car);
    for (auto vio : violations) {
        cout << "  Нарушение: " << vio->getDescription() << endl;

        if (!registry->isAlreadyProcessed(car->id, vio->getDescription())) {
            Resolution* res = new Resolution("RES_001", 1000);
            registry->registerFine(res, car->id, vio->getDescription());
            res->save();
            delete res;
        }
        delete vio;
    }

    // 6. Возвращаем камеры в пул
    cout << "\n--- ВОЗВРАТ КАМЕР В ПУЛ ---" << endl;
    for (auto cam : cameras) {
        camPool->releaseCamera(cam);
    }

    // 7. Очистка кадров
    for (auto f : frames) {
        delete f;
    }

    // 8. Очистка памяти
    delete zone;
    delete speedCreator60;
    delete speedCreator90;
    delete busCreator;
    delete car;

    camPool->showStats();
    registry->showStats();
}

// ============================================================================
// СЦЕНАРИЙ 2: Расширенная конфигурация
// ============================================================================
void demoScenario2() {
    printSeparator("СЦЕНАРИЙ 2: Расширенная конфигурация (Singleton + Factory Method + Abstract Factory + Object Pool)");

    FineRegistry* registry = FineRegistry::getInstance();
    SystemClock* clock = SystemClock::getInstance();

    // 1. СОЗДАЁМ ПУЛ КАМЕР И ВРУЧНУЮ ДОБАВЛЯЕМ КАМЕРЫ
    CameraPool* camPool = CameraPool::getInstance(5);

    cout << "\n--- РУЧНОЕ ДОБАВЛЕНИЕ КАМЕР В ПУЛ ---" << endl;
    for (int i = 1; i <= 5; i++) {
        camPool->addCamera(new Camera());
    }

    // 2. Abstract Factory: создаём компоненты для разных режимов
    cout << "\n--- РЕЖИМ: СТРОГИЙ ЦЕНТР ГОРОДА ---" << endl;
    AbstractZoneFactory* cityFactory = new StrictCityFactory();
    ControlZone* cityZone = cityFactory->createZone();
    EvidenceCollector* cityCollector = cityFactory->createEvidenceCollector();
    ResolutionGenerator* cityGenerator = cityFactory->createResolutionGenerator();

    cout << "\n--- РЕЖИМ: ЗАГОРОДНАЯ ТРАССА ---" << endl;
    AbstractZoneFactory* highwayFactory = new HighwayFactory();
    ControlZone* highwayZone = highwayFactory->createZone();
    EvidenceCollector* highwayCollector = highwayFactory->createEvidenceCollector();
    ResolutionGenerator* highwayGenerator = highwayFactory->createResolutionGenerator();

    // 3. Создаём тестовые автомобили
    PassengerCar* cityCar = new PassengerCar("CITY_001");
    cityCar->speed = 75;
    cityCar->lane = 1;
    cityCar->plate = new LicensePlate("C123AA", "77");

    PassengerCar* highwayCar = new PassengerCar("HWY_001");
    highwayCar->speed = 125;
    highwayCar->lane = 2;
    highwayCar->plate = new LicensePlate("H987BB", "50");

    // 4. Получаем камеры из пула
    cout << "\n--- ПОЛУЧЕНИЕ КАМЕР ИЗ ПУЛА ---" << endl;
    vector<Camera*> cameras;
    vector<Frame*> frames;

    for (int i = 0; i < 4; i++) {
        Camera* cam = camPool->acquireCamera();
        if (cam != nullptr) {
            cameras.push_back(cam);
            frames.push_back(cam->getFrame());
        }
    }

    // 5. Проверка в строгой зоне
    cout << "\n--- ПРОВЕРКА В СТРОГОЙ ЗОНЕ (центр города) ---" << endl;
    auto cityViolations = cityZone->check(cityCar);
    for (auto vio : cityViolations) {
        cout << "  Нарушение: " << vio->getDescription() << endl;
        if (!registry->isAlreadyProcessed(cityCar->id, vio->getDescription())) {
            Resolution* res = cityGenerator->generate(nullptr, cityCar->speed);
            if (res) {
                registry->registerFine(res, cityCar->id, vio->getDescription());
                res->save();
                delete res;
            }
        }
        delete vio;
    }

    // 6. Проверка в либеральной зоне
    cout << "\n--- ПРОВЕРКА В ЛИБЕРАЛЬНОЙ ЗОНЕ (трасса) ---" << endl;
    auto highwayViolations = highwayZone->check(highwayCar);
    for (auto vio : highwayViolations) {
        cout << "  Нарушение: " << vio->getDescription() << endl;
        if (!registry->isAlreadyProcessed(highwayCar->id, vio->getDescription())) {
            Resolution* res = highwayGenerator->generate(nullptr, highwayCar->speed);
            if (res) {
                registry->registerFine(res, highwayCar->id, vio->getDescription());
                res->save();
                delete res;
            }
        }
        delete vio;
    }

    // 7. Возвращаем камеры в пул
    cout << "\n--- ВОЗВРАТ КАМЕР В ПУЛ ---" << endl;
    for (auto cam : cameras) {
        camPool->releaseCamera(cam);
    }
    for (auto f : frames) {
        delete f;
    }

    // 8. Очистка памяти
    delete cityZone;
    delete highwayZone;
    delete cityFactory;
    delete highwayFactory;
    delete cityCar;
    delete highwayCar;

    camPool->showStats();
    registry->showStats();
}

// ============================================================================
// СЦЕНАРИЙ 3: Аналитическая конфигурация (Prototype)
// ============================================================================
void demoScenario3() {
    printSeparator("СЦЕНАРИЙ 3: Аналитическая конфигурация (Singleton + Factory Method + Prototype + Object Pool)");

    FineRegistry* registry = FineRegistry::getInstance();

    // 1. СОЗДАЁМ ПУЛ КАМЕР
    CameraPool* camPool = CameraPool::getInstance(3);
    camPool->addCamera(new Camera());
    camPool->addCamera(new Camera());
    camPool->addCamera(new Camera());

    // 2. ПРОТОТИП №1: SpeedViolationWithClone
    PassengerCar* templateCar = new PassengerCar("TEMPLATE_SPEED");
    templateCar->speed = 80;
    SpeedViolationWithClone* speedPrototype = new SpeedViolationWithClone(templateCar, 80, 60);
    cout << "\n[ПРОТОТИП 1] Создан прототип нарушения скорости: " << speedPrototype->getDescription() << endl;

    // 3. ПРОТОТИП №2: клонирование зоны через конструктор копирования
    ControlZone* originalZone = new ControlZone(100, false);

    SpeedRule* speedRule = new SpeedRule(60);
    BusLaneRule* busRule = new BusLaneRule(1);

    originalZone->addRule(speedRule);
    originalZone->addRule(busRule);
    cout << "[ПРОТОТИП 2] Создана оригинальная зона контроля с ID=" << originalZone->getZoneId() << endl;

    // Клонируем зону
    ControlZone* clonedZone = originalZone->clone();
    cout << "[ПРОТОТИП 2] Клон зоны имеет ID=" << clonedZone->getZoneId() << endl;

    // 4. Массовое клонирование нарушений для разных автомобилей
    vector<string> carIds = { "CAR_001", "CAR_002", "CAR_003", "CAR_004", "CAR_005" };
    vector<int> speeds = { 75, 82, 78, 95, 71 };

    cout << "\n=== МАССОВОЕ КЛОНИРОВАНИЕ НАРУШЕНИЙ (Prototype) ===" << endl;

    for (int i = 0; i < (int)carIds.size(); i++) {
        SpeedViolationWithClone* clonedViolation = speedPrototype->clone();

        PassengerCar* car = new PassengerCar(carIds[i]);
        car->speed = speeds[i];
        car->plate = new LicensePlate(carIds[i] + "XX", "77");

        cout << "\n  Автомобиль " << carIds[i] << ": скорость=" << speeds[i] << " км/ч" << endl;

        if (car->speed > 60) {
            cout << "    -> КЛОН нарушения: " << clonedViolation->getDescription() << endl;

            if (!registry->isAlreadyProcessed(carIds[i], "Speed")) {
                Resolution* res = new Resolution("RES_SPEED_" + carIds[i],
                    (car->speed > 80 ? 1500 : (car->speed > 70 ? 1000 : 500)));
                registry->registerFine(res, carIds[i], "Speed");
                res->save();
                delete res;
            }
        }

        delete clonedViolation;
        delete car;
    }

    // 5. Проверка в клонированной зоне
    cout << "\n=== ПРОВЕРКА АВТОМОБИЛЯ В КЛОНИРОВАННОЙ ЗОНЕ ===" << endl;

    PassengerCar* testCar = new PassengerCar("TEST_CAR");
    testCar->speed = 75;
    testCar->lane = 1;
    testCar->plate = new LicensePlate("TEST123", "99");

    auto violations = clonedZone->check(testCar);
    for (auto vio : violations) {
        cout << "  Клонированная зона (ID=" << clonedZone->getZoneId()
            << ") зафиксировала: " << vio->getDescription() << endl;
        delete vio;
    }

    // 6. Очистка памяти
    delete originalZone;
    delete clonedZone;
    delete speedRule;
    delete busRule;
    delete speedPrototype;
    delete templateCar;
    delete testCar;

    camPool->showStats();
    registry->showStats();
}

// ============================================================================
// СЦЕНАРИЙ 4: Полная конфигурация
// ============================================================================
void demoScenario4() {
    printSeparator("СЦЕНАРИЙ 4: Полная конфигурация (Все паттерны)");

    FineRegistry* registry = FineRegistry::getInstance();
    SystemClock* clock = SystemClock::getInstance();

    // 1. СОЗДАЁМ ПУЛ КАМЕР НА 10 КАМЕР
    CameraPool* camPool = CameraPool::getInstance(10);

    cout << "\n--- РУЧНОЕ ДОБАВЛЕНИЕ 10 КАМЕР В ПУЛ ---" << endl;
    for (int i = 1; i <= 10; i++) {
        camPool->addCamera(new Camera());
    }

    // 2. Abstract Factory: создаём компоненты для разных режимов
    cout << "\n--- ИНИЦИАЛИЗАЦИЯ КОМПОНЕНТОВ ЧЕРЕЗ ABSTRACT FACTORY ---" << endl;
    AbstractZoneFactory* cityFactory = new StrictCityFactory();
    ControlZone* cityZone = cityFactory->createZone();

    AbstractZoneFactory* highwayFactory = new HighwayFactory();
    ControlZone* highwayZone = highwayFactory->createZone();

    // 3. Прототип для массового клонирования нарушений
    PassengerCar* templateCar = new PassengerCar("TEMPLATE");
    templateCar->speed = 85;
    SpeedViolationWithClone* speedPrototype = new SpeedViolationWithClone(templateCar, 85, 60);

    // 4. Прототип зоны для клонирования
    ControlZone* templateZone = new ControlZone(1000, false);
    SpeedRule* templateSpeedRule = new SpeedRule(60);
    BusLaneRule* templateBusRule = new BusLaneRule(1);
    templateZone->addRule(templateSpeedRule);
    templateZone->addRule(templateBusRule);
    cout << "\n[Prototype] Создана зона-шаблон с ID=" << templateZone->getZoneId() << endl;

    // 5. Моделирование трафика в центре города
    cout << "\n=== МОДЕЛИРОВАНИЕ ТРАФИКА В ЦЕНТРЕ ГОРОДА ===" << endl;

    vector<PassengerCar*> cityCars;
    for (int i = 0; i < 10; i++) {
        PassengerCar* car = new PassengerCar("CITY_" + to_string(i + 1));
        car->speed = 60 + (rand() % 50);
        car->lane = (rand() % 3) + 1;
        car->plate = new LicensePlate("C" + to_string(i + 1) + "AA", "77");
        cityCars.push_back(car);
    }

    // Получаем камеры из пула
    cout << "\n--- ПОЛУЧЕНИЕ КАМЕР ИЗ ПУЛА ---" << endl;
    vector<Camera*> cameras;
    for (int i = 0; i < 8; i++) {
        Camera* cam = camPool->acquireCamera();
        if (cam != nullptr) {
            cameras.push_back(cam);
        }
    }

    int violationCount = 0;
    for (auto car : cityCars) {
        auto violations = cityZone->check(car);
        for (auto vio : violations) {
            violationCount++;
            cout << "  ТС " << car->id << ": " << vio->getDescription()
                << " (скорость " << car->speed << " км/ч)" << endl;

            SpeedViolationWithClone* clonedViolation = speedPrototype->clone();

            if (!registry->isAlreadyProcessed(car->id, "Speed")) {
                Resolution* res = new Resolution("RES_" + car->id,
                    (car->speed > 80 ? 1500 : (car->speed > 70 ? 1000 : 500)));
                registry->registerFine(res, car->id, "Speed");
                res->save();
                delete res;
            }

            delete clonedViolation;
            delete vio;
        }
    }

    // 6. Клонирование зоны для нового перекрёстка
    cout << "\n=== КЛОНИРОВАНИЕ ЗОНЫ ДЛЯ НОВОГО ПЕРЕКРЁСТКА ===" << endl;
    ControlZone* newIntersection = templateZone->clone();
    cout << "Создан новый перекрёсток с ID=" << newIntersection->getZoneId()
        << " (клон зоны-шаблона ID=" << templateZone->getZoneId() << ")" << endl;

    // 7. Возвращаем камеры в пул
    cout << "\n--- ВОЗВРАТ КАМЕР В ПУЛ ---" << endl;
    for (auto cam : cameras) {
        camPool->releaseCamera(cam);
    }

    // 8. Очистка памяти
    for (auto car : cityCars) delete car;
    delete cityZone;
    delete highwayZone;
    delete cityFactory;
    delete highwayFactory;
    delete templateZone;
    delete newIntersection;
    delete templateSpeedRule;
    delete templateBusRule;
    delete speedPrototype;
    delete templateCar;

    cout << "\n=== ИТОГИ ПОЛНОЙ КОНФИГУРАЦИИ ===" << endl;
    camPool->showStats();
    registry->showStats();
    cout << "  Всего обработано нарушений: " << violationCount << endl;
}

int main() {
    srand(time(nullptr));
    setlocale(LC_ALL, "Rus");

    cout << "     СИСТЕМА ФОТОВИДЕОФИКСАЦИИ НАРУШЕНИЙ ПДД" << endl;
    cout << "     Демонстрация порождающих паттернов" << endl;

    demoScenario1();
    demoScenario2();
    demoScenario3();
    demoScenario4();

    cout << "ЗАВЕРШЕНИЕ РАБОТЫ СИСТЕМЫ" << endl;
    return 0;
}