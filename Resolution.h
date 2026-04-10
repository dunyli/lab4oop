#pragma once
#include <string>
#include <iostream>
#include <ctime>
using namespace std;

class Resolution {
private:
    string id;
    int amount;
    string status;

public:
    Resolution(string i, int a) : id(i), amount(a), status("issued") {}

    void save() {
        cout << "  [Resolution] Сохранено постановление " << id
            << ", сумма: " << amount << " руб." << endl;
    }

    string getId() const { return id; }
    int getAmount() const { return amount; }
};