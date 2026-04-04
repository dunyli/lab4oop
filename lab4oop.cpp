#include <iostream>
#include <vector>
#include "Vehicle.h"
#include "ControlZone.h"
#include "RuleFactory.h"
#include "IViolationExpert.h"
#include "Camera.h"
#include "Frame.h"

// ===== ПАТТЕРН BRIDGE =====
#include "VideoSource.h"
#include "CameraSource.h"
#include "FileSource.h"
#include "VideoHandler.h"
#include "ViolationHandler.h"
#include "ArchiveHandler.h"

// ===== ПАТТЕРН FACADE =====
#include "TrafficMonitorFacade.h"

// ===== ДВА ЭКСПЕРТА =====
#include "StatisticsExpert.h"
#include "FineCalculatorExpert.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");

    cout << "СИСТЕМА ФИКСАЦИИ НАРУШЕНИЙ ПДД" << endl;

    // СЦЕНАРИЙ 1: КОНФИГУРАЦИЯ «БАЗОВАЯ» (ТОЛЬКО FLYWEIGHT)
    // Цель: минимально работоспособная система с экономией памяти

    cout << "\n==============================================================" << endl;
    cout << "СЦЕНАРИЙ 1: БАЗОВАЯ КОНФИГУРАЦИЯ (Только Flyweight)" << endl;
    cout << "Назначение: простые участки дорог, экономия памяти" << endl;
    cout << "Применённые паттерны: Flyweight" << endl;

    // Создаём фабрику приспособленцев (Flyweight)
    RuleFactory factory;

    // Получаем разделяемые правила - ОДИН объект на все зоны с одинаковым лимитом
    SpeedRule* speed60 = factory.getSpeedRule(60, "Базовая зона");
    SpeedRule* speed40 = factory.getSpeedRule(40, "Школьная зона");
    SpeedRule* speed60_2 = factory.getSpeedRule(60, "Вторая зона"); // ТОТ ЖЕ объект!

    // Создаём зоны контроля (неразделяемые приспособленцы)
    // Используем конструктор без эксперта (deleteViolations = true)
    ControlZone baseZone1("Z001", "Перекресток Ленина");
    ControlZone baseZone2("Z002", "Школа Пушкина");

    // Добавляем разделяемые правила в зоны
    baseZone1.addRule(speed60);
    baseZone2.addRule(speed40);

    // Проверяем автомобили
    Vehicle car1("A123BC", "PassengerCar");
    car1.speed = 75;
    car1.lane = 1;

    Vehicle car2("B456DE", "PassengerCar");
    car2.speed = 45;
    car2.lane = 2;

    cout << "\nПроверка автомобилей" << endl;
    baseZone1.printCheck(&car1);
    baseZone2.printCheck(&car2);

    // Демонстрация экономии памяти Flyweight
    cout << "\nЭкономия памяти Flyweight" << endl;
    cout << "SpeedRule(60) создан 1 раз: " << speed60 << endl;
    cout << "SpeedRule(60) повторно получен: " << speed60_2 << " (ТОТ ЖЕ ОБЪЕКТ)" << endl;
    cout << "Уникальных SpeedRule: " << factory.getSpeedRuleCount() << endl;
    cout << "Уникальных BusLaneRule: " << factory.getBusLaneRuleCount() << endl;


    // СЦЕНАРИЙ 2: КОНФИГУРАЦИЯ «РАСШИРЕННАЯ» (FLYWEIGHT + BRIDGE)
    // Цель: гибкая работа с разными источниками видео и обработчиками

    cout << "\n==============================================================" << endl;
    cout << "СЦЕНАРИЙ 2: РАСШИРЕННАЯ КОНФИГУРАЦИЯ (Flyweight + Bridge)" << endl;
    cout << "Назначение: гибкое подключение источников и обработчиков видео" << endl;
    cout << "Применённые паттерны: Flyweight, Bridge" << endl;

    // Создаём камеру (старый класс)
    Camera camera;

    // BRIDGE: РЕАЛИЗАЦИИ (источники видео)
    CameraSource camSource("CAM_001", &camera);
    FileSource fileSource("archive.dat");

    // BRIDGE: АБСТРАКЦИИ (обработчики)
    // Создаём зону для обработчика нарушений (без эксперта)
    ControlZone bridgeZone("Z003", "Перекресток Мира");
    bridgeZone.addRule(factory.getSpeedRule(60, "Перекресток Мира"));
    bridgeZone.addRule(factory.getBusLaneRule(1, "Перекресток Мира"));

    // Создаём обработчики
    ViolationHandler violationHandler(&camSource, &bridgeZone);
    ArchiveHandler archiveHandler(&fileSource);

    // Проверяем автомобиль через разные комбинации Bridge
    Vehicle car3("C789FG", "PassengerCar");
    car3.speed = 82;
    car3.lane = 1;

    cout << "\nКомбинация 1: ViolationHandler + CameraSource" << endl;
    violationHandler.process(&car3);

    cout << "\nКомбинация 2: ArchiveHandler + FileSource" << endl;
    archiveHandler.process(&car3);

    // Демонстрация гибкости Bridge - смена источника на лету
    cout << "\nДемонстрация смены источника (динамически)" << endl;
    cout << "Смена с CameraSource на FileSource:" << endl;
    violationHandler.setSource(&fileSource);
    violationHandler.process(&car3);

    cout << "\nВсе комбинации Bridge (2×2 = 4)" << endl;
    cout << "1. ViolationHandler + CameraSource" << endl;
    cout << "2. ViolationHandler + FileSource" << endl;
    cout << "3. ArchiveHandler + CameraSource" << endl;
    cout << "4. ArchiveHandler + FileSource" << endl;


    // СЦЕНАРИЙ 3: КОНФИГУРАЦИЯ «УПРОЩЁННАЯ» (FLYWEIGHT + FACADE)
    // Цель: простой интерфейс для клиента, скрывающий сложность системы

    cout << "\n==============================================================" << endl;
    cout << "СЦЕНАРИЙ 3: УПРОЩЁННАЯ КОНФИГУРАЦИЯ (Flyweight + Facade)" << endl;
    cout << "Назначение: простой интерфейс для быстрой разработки" << endl;
    cout << "Применённые паттерны: Flyweight, Facade" << endl;

    // Создаём фасад (скрывает всю сложность внутри)
    TrafficMonitorFacade facade(&camera);

    // Создаём зоны через фасад - просто и понятно
    facade.createSpeedZone("Z004", "Фасадная зона 1", 60);
    facade.createBusLaneZone("Z005", "Фасадная зона 2", 1, 60);

    // Проверяем автомобиль через фасад - один вызов вместо множества
    Vehicle car4("D901EF", "PassengerCar");
    car4.speed = 78;
    car4.lane = 1;

    cout << "\nПроверка через фасад" << endl;
    facade.checkVehicleInAllZones(&car4);

    // Полная проверка с захватом кадра
    cout << "\nПолная проверка с захватом кадра" << endl;
    facade.captureAndCheck("E123FG", 85, 1, "PassengerCar");

    // Список всех зон
    facade.printAllZones();

    // Быстрые методы фасада
    cout << "\nБыстрые методы фасада" << endl;
    cout << "Превышение 95 > 60: " << (facade.isSpeedViolation(95, 60) ? "Да" : "Нет") << endl;
    cout << "Штраф: " << facade.calculateFine(95, 60) << " руб." << endl;


    // СЦЕНАРИЙ 4: КОНФИГУРАЦИЯ «АНАЛИТИЧЕСКАЯ» (FLYWEIGHT + INFORMATION EXPERT)
    // Цель: сбор статистики, учёт истории нарушений
    cout << "\n==============================================================" << endl;
    cout << "СЦЕНАРИЙ 4: АНАЛИТИЧЕСКАЯ КОНФИГУРАЦИЯ (Flyweight + Information Expert)" << endl;
    cout << "Назначение: сбор статистики, учёт истории нарушений" << endl;
    cout << "Применённые паттерны: Flyweight, Information Expert" << endl;

    // ===== ИСПОЛЬЗОВАНИЕ ДВУХ ЭКСПЕРТОВ =====

    // ЭКСПЕРТ 1: ДЛЯ СБОРА СТАТИСТИКИ
    StatisticsExpert statExpert;

    // ЭКСПЕРТ 2: ДЛЯ РАСЧЁТА ШТРАФОВ
    FineCalculatorExpert fineExpert;

    // Создаём зоны с разными экспертами
    ControlZone statZone("Z006", "Статистическая зона", &statExpert);
    ControlZone fineZone("Z007", "Штрафная зона", &fineExpert);

    statZone.addRule(factory.getSpeedRule(60, "Статистическая зона"));
    statZone.addRule(factory.getBusLaneRule(1, "Статистическая зона"));
    fineZone.addRule(factory.getSpeedRule(40, "Штрафная зона"));

    // Проверяем автомобили (нарушения попадают к разным экспертам)
    Vehicle car5("X999XX", "PassengerCar");
    car5.speed = 75;
    car5.lane = 1;

    Vehicle car6("X999XX", "PassengerCar");  // ТОТ ЖЕ автомобиль!
    car6.speed = 82;
    car6.lane = 1;

    Vehicle car7("Y777YY", "PassengerCar");
    car7.speed = 45;
    car7.lane = 2;

    cout << "\nПроверка автомобилей (с передачей нарушений разным экспертам)" << endl;
    statZone.printCheck(&car5);   // идёт в StatisticsExpert
    statZone.printCheck(&car6);   // идёт в StatisticsExpert
    fineZone.printCheck(&car7);   // идёт в FineCalculatorExpert

    // Получение информации от каждого эксперта
    cout << "\nИНФОРМАЦИЯ ОТ ЭКСПЕРТА СТАТИСТИКИ" << endl;
    cout << statExpert.getInfo() << endl;

    cout << "\nИНФОРМАЦИЯ ОТ ЭКСПЕРТА ШТРАФОВ" << endl;
    cout << fineExpert.getInfo() << endl;


    // СЦЕНАРИЙ 5: КОНФИГУРАЦИЯ «ПОЛНАЯ» (ВСЕ ПАТТЕРНЫ ВМЕСТЕ)

   // Создаём все компоненты (используем конкретные классы)
    StatisticsExpert fullStatExpert;
    FineCalculatorExpert fullFineExpert;
    RuleFactory fullFactory;
    Camera fullCamera;

    // Создаём фасад (без эксперта, для простых операций)
    TrafficMonitorFacade fullFacade(&fullCamera);

    // Через фасад создаём зоны (без эксперта)
    fullFacade.createSpeedZone("Z008", "Полная зона 1", 60);
    fullFacade.createBusLaneZone("Z009", "Полная зона 2", 1, 60);

    // Создаём дополнительные зоны напрямую с экспертами
    ControlZone fullZoneStat("Z010", "Полная зона статистики", &fullStatExpert);
    fullZoneStat.addRule(fullFactory.getSpeedRule(40, "Полная зона статистики"));

    ControlZone fullZoneFine("Z011", "Полная зона штрафов", &fullFineExpert);
    fullZoneFine.addRule(fullFactory.getSpeedRule(50, "Полная зона штрафов"));

    // Создаём Bridge компоненты
    CameraSource fullCamSource("FULL_CAM", &fullCamera);
    ViolationHandler fullHandlerStat(&fullCamSource, &fullZoneStat);
    ViolationHandler fullHandlerFine(&fullCamSource, &fullZoneFine);

    // Проверяем автомобили
    Vehicle fullCar1("MASTER_01", "PassengerCar");
    fullCar1.speed = 78;
    fullCar1.lane = 1;

    Vehicle fullCar2("MASTER_01", "PassengerCar");
    fullCar2.speed = 85;
    fullCar2.lane = 1;

    Vehicle fullCar3("SLAVE_01", "PassengerCar");
    fullCar3.speed = 55;
    fullCar3.lane = 2;

    cout << "\nПроверка через Bridge с экспертом статистики" << endl;
    fullHandlerStat.process(&fullCar1);
    fullHandlerStat.process(&fullCar2);

    cout << "\nПроверка через Bridge с экспертом штрафов" << endl;
    fullHandlerFine.process(&fullCar3);

    cout << "\nПроверка через фасад" << endl;
    fullFacade.checkVehicleInAllZones(&fullCar1);

    // Полная статистика от экспертов
    cout << "\nСтатистика от эксперта StatisticsExpert" << endl;
    cout << fullStatExpert.getInfo() << endl;

    cout << "\nСтатистика от эксперта FineCalculatorExpert" << endl;
    cout << fullFineExpert.getInfo() << endl;

    // ИТОГОВАЯ СВОДКА

    cout << "ПРЕИМУЩЕСТВА ПАТТЕРНОВ" << endl;
    cout << "1. Flyweight: экономия памяти (1 объект SpeedRule на 100 зон)" << endl;
    cout << "2. Bridge: гибкая подмена источников и обработчиков видео" << endl;
    cout << "3. Facade: простой интерфейс для клиента" << endl;
    cout << "4. Information Expert: централизованная статистика и учёт истории" << endl;
    cout << "   - StatisticsExpert: сбор статистики по зонам и нарушителям" << endl;
    cout << "   - FineCalculatorExpert: расчёт штрафов с учётом истории" << endl;

    cout << "РАБОТА ЗАВЕРШЕНА" << endl;

    return 0;
}