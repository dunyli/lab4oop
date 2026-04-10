#pragma once
#include "Resolution.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

/**
 * FineRegistry - центральный реестр выписанных штрафов (Singleton)
 * Гарантирует существование единственного экземпляра для регистрации
 * всех постановлений о штрафах и предотвращения дублирования наказаний
 * за одно и то же нарушение одним транспортным средством.
 */
class FineRegistry {
private:
    static FineRegistry* instance;

    // Коллекция всех выписанных штрафов
    vector<Resolution*> fines;

    // Карта для быстрой проверки дублей: ключ = "vehicleId_violationType"
    map<string, bool> processedViolations;

    // Статистика по типам нарушений
    map<string, int> violationTypeStats;

    // Закрытый конструктор (Singleton)
    FineRegistry() {
        cout << "[FineRegistry] Инициализация центрального реестра штрафов" << endl;
    }

    // Закрытый конструктор копирования
    FineRegistry(const FineRegistry&) = delete;

    // Закрытый оператор присваивания
    FineRegistry& operator=(const FineRegistry&) = delete;

public:
    /**
     * Получение единственного экземпляра реестра
     */
    static FineRegistry* getInstance() {
        if (instance == nullptr) {
            instance = new FineRegistry();
        }
        return instance;
    }

    /**
     * Проверка, не было ли уже выписано постановление
     * за данное нарушение для указанного транспортного средства
     * vehicleId - идентификатор транспортного средства
     * violationType - тип нарушения
     * true - если штраф уже выписан, false - если нет
     */
    bool isAlreadyProcessed(const string& vehicleId, const string& violationType) {
        string key = vehicleId + "_" + violationType;
        if (processedViolations.find(key) != processedViolations.end()) {
            cout << "  [FineRegistry] ВНИМАНИЕ: Штраф за " << violationType
                << " для ТС " << vehicleId << " уже выписан!" << endl;
            return true;
        }
        return false;
    }

    /**
     * Регистрация нового штрафа в реестре
     * resolution - постановление о штрафе
     * vehicleId - идентификатор транспортного средства-нарушителя
     * violationType - тип нарушения
     */
    void registerFine(Resolution* resolution, const string& vehicleId, const string& violationType) {
        if (resolution == nullptr) {
            cout << "  [FineRegistry] ОШИБКА: Попытка зарегистрировать пустой штраф" << endl;
            return;
        }

        string key = vehicleId + "_" + violationType;

        // Проверяем, не зарегистрирован ли уже такой штраф
        if (processedViolations.find(key) != processedViolations.end()) {
            cout << "  [FineRegistry] ОШИБКА: Штраф для " << vehicleId
                << " за " << violationType << " уже существует!" << endl;
            delete resolution;  // Удаляем дубликат
            return;
        }

        // Регистрируем штраф
        fines.push_back(resolution);
        processedViolations[key] = true;
        violationTypeStats[violationType]++;

        cout << "  [FineRegistry] Штраф " << resolution->getId()
            << " зарегистрирован для ТС " << vehicleId
            << " (тип: " << violationType << ")" << endl;
    }

    int getTotalFinesCount() const {
        return fines.size();
    }

    /*Получение статистики по типам нарушений*/
    string getViolationStatistics() {
        string stats = "\nСТАТИСТИКА НАРУШЕНИЙ\n";
        stats += "Всего штрафов: " + to_string(fines.size()) + "\n";
        stats += "По типам нарушений:\n";

        for (const auto& pair : violationTypeStats) {
            stats += "  - " + pair.first + ": " + to_string(pair.second) + "\n";
        }

        return stats;
    }

    /**
     * Вывод статистики в консоль
     */
    void showStats() {
        cout << "\n  [FineRegistry] Статистика реестра штрафов:" << endl;
        cout << "    Всего выписано штрафов: " << fines.size() << endl;
        cout << "    Уникальных нарушений (без дублей): " << processedViolations.size() << endl;
    }

    /**
     * Получение всех зарегистрированных штрафов
     */
    vector<Resolution*> getAllFines() const {
        return fines;
    }

    /**
     * Очистка реестра (удаление всех штрафов)
     */
    void clear() {
        for (auto fine : fines) {
            delete fine;
        }
        fines.clear();
        processedViolations.clear();
        violationTypeStats.clear();
        cout << "[FineRegistry] Реестр штрафов очищен" << endl;
    }

    /**
     * Уничтожение единственного экземпляра реестра
     */
    static void destroy() {
        if (instance != nullptr) {
            instance->clear();
            delete instance;
            instance = nullptr;
            cout << "[FineRegistry] Экземпляр реестра уничтожен" << endl;
        }
    }
    ~FineRegistry() {
        clear();
    }
};

// Инициализация статического члена класса
FineRegistry* FineRegistry::instance = nullptr;