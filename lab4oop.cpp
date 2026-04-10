#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

// Подключение всех компонентов системы
#include "Frame.h"
#include "FramePool.h"
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
#include "ControlZoneWithPrototype.h"
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

// Демонстрация Сценария 1: Базовая конфигурация
void demoScenario1() {
    printSeparator("СЦЕНАРИЙ 1: Базовая конфигурация (Singleton + Factory Method + Object Pool)");

    FineRegistry* registry = FineRegistry::getInstance();
    FramePool* pool = FramePool::getInstance(50);

    ControlZone* zone = new ControlZone(1, false);

    // Создаём правила
    SpeedRule* speedRule60 = new SpeedRule(60);
    BusLaneRule* busRule = new BusLaneRule(5);

    zone->addRule(speedRule60);
    zone->addRule(busRule);

    cout << "\nПроверка автомобилей" << endl;

    vector<Frame*> frames;
    for (int i = 0; i < 5; i++) {
        Frame* f = pool->acquireFrame(1000 + i);
        frames.push_back(f);
    }

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

    for (auto f : frames) {
        pool->releaseFrame(f);
    }

    // Удаляем зону
    delete zone;

    // Вручную удаляем правила
    delete speedRule60;
    delete busRule;
    delete car;

    registry->showStats();
    pool->showStats();
}

// Демонстрация Сценария 2: Расширенная конфигурация
void demoScenario2() {
    printSeparator("СЦЕНАРИЙ 2: Расширенная конфигурация (Singleton + Factory Method + Abstract Factory + Object Pool)");

    FineRegistry* registry = FineRegistry::getInstance();
    SystemClock* clock = SystemClock::getInstance();
    FramePool* pool = FramePool::getInstance(100);

    cout << "\nРежим: СТРОГИЙ ЦЕНТР ГОРОДА " << endl;

    AbstractZoneFactory* cityFactory = new StrictCityFactory();
    ControlZone* cityZone = cityFactory->createZone();  // зона будет удалять правила
    EvidenceCollector* cityCollector = cityFactory->createEvidenceCollector();
    ResolutionGenerator* cityGenerator = cityFactory->createResolutionGenerator();

    cout << "\nРежим: ЗАГОРОДНАЯ ТРАССА " << endl;

    AbstractZoneFactory* highwayFactory = new HighwayFactory();
    ControlZone* highwayZone = highwayFactory->createZone();  // зона будет удалять правила
    EvidenceCollector* highwayCollector = highwayFactory->createEvidenceCollector();
    ResolutionGenerator* highwayGenerator = highwayFactory->createResolutionGenerator();

    PassengerCar* cityCar = new PassengerCar("CITY_001");
    cityCar->speed = 75;
    cityCar->lane = 1;
    cityCar->plate = new LicensePlate("C123AA", "77");

    PassengerCar* highwayCar = new PassengerCar("HWY_001");
    highwayCar->speed = 125;
    highwayCar->lane = 2;
    highwayCar->plate = new LicensePlate("H987BB", "50");

    vector<Frame*> frames;
    for (int i = 0; i < 7; i++) {
        frames.push_back(pool->acquireFrame(clock->getRealTime() + i));
    }

    cout << "\nПроверка в строгой зоне (центр города)" << endl;
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

    cout << "\nПроверка в либеральной зоне (трасса)" << endl;
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

    for (auto f : frames) {
        pool->releaseFrame(f);
    }

    delete cityZone;
    delete highwayZone;
    delete cityFactory;
    delete highwayFactory;
    delete cityCar;
    delete highwayCar;

    registry->showStats();
    pool->showStats();
}

// Демонстрация Сценария 3: Аналитическая конфигурация (Prototype)
void demoScenario3() {
    printSeparator("СЦЕНАРИЙ 3: Аналитическая конфигурация (Singleton + Factory Method + Prototype + Object Pool)");

    FineRegistry* registry = FineRegistry::getInstance();
    FramePool* pool = FramePool::getInstance(50);

    // ПРОТОТИП №1: SpeedViolationWithClone 
    PassengerCar* templateCar = new PassengerCar("TEMPLATE_SPEED");
    templateCar->speed = 80;
    SpeedViolationWithClone* speedPrototype = new SpeedViolationWithClone(templateCar, 80, 60);
    cout << "\n[ПРОТОТИП 1] Создан прототип нарушения скорости: " << speedPrototype->getDescription() << endl;

    // ПРОТОТИП №2: ControlZoneWithPrototype
    ControlZoneWithPrototype* originalZone = new ControlZoneWithPrototype(100);
    originalZone->setOwnsRules(false);  

    // Создаём правила отдельно
    SpeedRule* speedRule = new SpeedRule(60);
    BusLaneRule* busRule = new BusLaneRule(1);

    originalZone->addRule(speedRule);
    originalZone->addRule(busRule);
    cout << "[ПРОТОТИП 2] Создана оригинальная зона контроля с ID=" << originalZone->getZoneId() << endl;

    // Клонируем зону (клон наследует настройки владения)
    ControlZoneWithPrototype* clonedZone = originalZone->clone();
    cout << "[ПРОТОТИП 2] Клон зоны имеет ID=" << clonedZone->getZoneId() << " (оригинал + 1000)" << endl;
    cout << clonedZone->getRulesCount() << endl;
    // Массовое клонирование нарушений для разных автомобилей
    vector<string> carIds = { "CAR_001", "CAR_002", "CAR_003", "CAR_004", "CAR_005" };
    vector<int> speeds = { 75, 82, 78, 95, 71 };

    cout << "\nМАССОВОЕ КЛОНИРОВАНИЕ НАРУШЕНИЙ (Prototype)" << endl;

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

    // Проверка в клонированной зоне
    cout << "\nПРОВЕРКА АВТОМОБИЛЯ В КЛОНИРОВАННОЙ ЗОНЕ" << endl;

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

    // Очистка: удаляем зоны (они НЕ удаляют правила)
    delete originalZone;
    delete clonedZone;

    delete speedPrototype;
    delete templateCar;
    delete testCar;

    registry->showStats();
    pool->showStats();
}

// Демонстрация Сценария 4: Полная конфигурация
void demoScenario4() {
    printSeparator("СЦЕНАРИЙ 4: Полная конфигурация (Все паттерны)");

    FineRegistry* registry = FineRegistry::getInstance();
    SystemClock* clock = SystemClock::getInstance();
    FramePool* pool = FramePool::getInstance(200);

    cout << "\nИнициализация компонентов через Abstract Factory" << endl;

    // Фабрики создают зоны, которые САМИ удаляют правила
    AbstractZoneFactory* cityFactory = new StrictCityFactory();
    ControlZone* cityZone = cityFactory->createZone();

    AbstractZoneFactory* highwayFactory = new HighwayFactory();
    ControlZone* highwayZone = highwayFactory->createZone();

    // Создаём прототип для массового клонирования нарушений
    PassengerCar* templateCar = new PassengerCar("TEMPLATE");
    templateCar->speed = 85;
    SpeedViolationWithClone* speedPrototype = new SpeedViolationWithClone(templateCar, 85, 60);

    // Создаём зону-шаблон для клонирования (НЕ удаляет правила)
    ControlZoneWithPrototype* templateZone = new ControlZoneWithPrototype(1000);
    templateZone->setOwnsRules(false);

    SpeedRule* templateSpeedRule = new SpeedRule(60);
    BusLaneRule* templateBusRule = new BusLaneRule(1);
    templateZone->addRule(templateSpeedRule);
    templateZone->addRule(templateBusRule);
    cout << "[Prototype] Создана зона-шаблон с ID=" << templateZone->getZoneId() << endl;

    cout << "\nМоделирование трафика в центре города" << endl;

    vector<PassengerCar*> cityCars;
    for (int i = 0; i < 10; i++) {
        PassengerCar* car = new PassengerCar("CITY_" + to_string(i + 1));
        car->speed = 60 + (rand() % 50);
        car->lane = (rand() % 3) + 1;
        car->plate = new LicensePlate("C" + to_string(i + 1) + "AA", "77");
        cityCars.push_back(car);
    }

    vector<Frame*> frames;
    for (int i = 0; i < 10; i++) {
        frames.push_back(pool->acquireFrame(clock->getRealTime() + i));
    }

    int violationCount = 0;
    for (auto car : cityCars) {
        auto violations = cityZone->check(car);
        for (auto vio : violations) {
            violationCount++;
            cout << "  ТС " << car->id << ": " << vio->getDescription()
                << " (скорость " << car->speed << " км/ч)" << endl;

            SpeedViolationWithClone* clonedViolation = speedPrototype->clone();
            cout << "    -> Использован клон нарушения: " << clonedViolation->getDescription() << endl;

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

    // Клонирование зоны для нового перекрёстка
    cout << "\nКЛОНИРОВАНИЕ ЗОНЫ ДЛЯ НОВОГО ПЕРЕКРЁСТКА " << endl;
    ControlZoneWithPrototype* newIntersection = templateZone->clone();
    cout << "Создан новый перекрёсток с ID=" << newIntersection->getZoneId()
        << " (клон зоны-шаблона ID=" << templateZone->getZoneId() << ")" << endl;

    PassengerCar* anotherCar = new PassengerCar("NEW_CAR");
    anotherCar->speed = 72;
    anotherCar->lane = 1;
    anotherCar->plate = new LicensePlate("NEW999", "99");

    auto newViolations = newIntersection->check(anotherCar);
    for (auto vio : newViolations) {
        cout << "  Клонированная зона зафиксировала: " << vio->getDescription() << endl;
        delete vio;
    }

    for (auto f : frames) {
        pool->releaseFrame(f);
    }

    // Очистка
    for (auto car : cityCars) delete car;

    // Удаляем зоны из фабрик
    delete cityZone;
    delete highwayZone;
    delete cityFactory;
    delete highwayFactory;

    // Удаляем зону-шаблон и её клон 
    delete templateZone;
    delete newIntersection;


    cout << "\nИТОГИ ПОЛНОЙ КОНФИГУРАЦИИ" << endl;
    registry->showStats();
    pool->showStats();
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