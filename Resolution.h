#pragma once
#include <string>
#include <iostream>

using namespace std;

// Класс данных - постановление о штрафе
class Resolution {
public:
    string id;
    int amount;

    Resolution(string i, int a) : id(i), amount(a) {}

    void save() {
        cout << "  [Resolution] Сохранено постановление " << id << ", сумма: " << amount << " руб." << endl;
    }
};