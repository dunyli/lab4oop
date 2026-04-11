#pragma once
#include "Camera.h"
#include <vector>
#include <iostream>
using namespace std;

/**
 * CameraPool - пул объектов камер (Object Pool)
 * Позволяет переиспользовать ограниченное количество камер,
 * вместо создания новых каждый раз
 */
class CameraPool {
private:
    static CameraPool* instance;     // Singleton: единственный экземпляр
    vector<Camera*> pool;            // СВОБОДНЫЕ камеры (ждут использования)
    vector<Camera*> allCameras;      // ВСЕ созданные камеры (для очистки)
    int maxSize;                     // Максимальный размер пула
    int createdCount;                // Сколько камер уже создано

    // Закрытый конструктор (Singleton)
    CameraPool(int max = 10) : maxSize(max), createdCount(0) {
        cout << "[CameraPool] Создан пул камер. Максимальный размер: " << maxSize << endl;
    }

public:
    // Получение единственного экземпляра пула
    static CameraPool* getInstance(int max = 10) {
        if (instance == nullptr) {
            instance = new CameraPool(max);
        }
        return instance;
    }

    /**
     * ДОБАВЛЕНИЕ КАМЕРЫ В ПУЛ (вручную)
     * cam - указатель на камеру
     * true - камера добавлена, false - пул заполнен
     */
    bool addCamera(Camera* cam) {
        if (cam == nullptr) {
            cout << "[CameraPool] Ошибка: попытка добавить nullptr" << endl;
            return false;
        }

        if (createdCount >= maxSize) {
            cout << "[CameraPool] Нельзя добавить камеру: пул достиг максимума ("
                << maxSize << ")" << endl;
            delete cam;  // удаляем, чтобы не было утечки
            return false;
        }

        allCameras.push_back(cam);
        pool.push_back(cam);  // кладём в пул свободных
        createdCount++;

        cout << "[CameraPool] Добавлена камера. Всего в пуле: " << createdCount
            << "/" << maxSize << ", свободных: " << pool.size() << endl;
        return true;
    }

    //ВЗЯТЬ камеру из пула
    Camera* acquireCamera() {
        // Если в пуле есть свободные камеры
        if (!pool.empty()) {
            Camera* cam = pool.back();   // берём последнюю
            pool.pop_back();              // удаляем из списка свободных
            cout << "  [CameraPool] Выдана камера. Свободных осталось: "
                << pool.size() << "/" << createdCount << endl;
            return cam;
        }

        // Если пул пуст
        cout << "  [CameraPool] ВНИМАНИЕ: пул камер пуст! Все " << createdCount
            << " камер заняты." << endl;
        return nullptr;
    }

    // ВЕРНУТЬ камеру в пул
    void releaseCamera(Camera* cam) {
        if (cam != nullptr) {
            pool.push_back(cam);
            cout << "  [CameraPool] Камера возвращена. Свободных камер: "
                << pool.size() << "/" << createdCount << endl;
        }
    }

    // Получить количество свободных камер
    int getFreeCount() const {
        return pool.size();
    }

    // Получить общее количество камер в пуле
    int getTotalCount() const {
        return createdCount;
    }

     // Показать статистику пула
    void showStats() {
        cout << "\nСТАТИСТИКА ПУЛА КАМЕР" << endl;
        cout << "  Максимальный размер пула: " << maxSize << endl;
        cout << "  Всего создано камер: " << createdCount << endl;
        cout << "  Свободных камер: " << pool.size() << endl;
        cout << "  Занятых камер: " << (createdCount - pool.size()) << endl;
    }


    // Очистка пула (удаление всех камер)
    void clear() {
        for (auto cam : allCameras) {
            delete cam;
        }
        allCameras.clear();
        pool.clear();
        createdCount = 0;
        cout << "[CameraPool] Пул камер очищен" << endl;
    }

    /**
     * Уничтожение Singleton
     */
    static void destroy() {
        if (instance != nullptr) {
            instance->clear();
            delete instance;
            instance = nullptr;
            cout << "[CameraPool] Экземпляр пула уничтожен" << endl;
        }
    }
};
CameraPool* CameraPool::instance = nullptr;