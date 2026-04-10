#include "EvidenceCollector.h"
#include "Evidence.h"
#include "Frame.h"         
#include "Violation.h"      
#include <iostream>
#include <ctime>
#include <string>
#include <vector>           

using namespace std;

Evidence* EvidenceCollector::collect(Violation* v, vector<Frame*>& frames, int quality, long violationTime) {
    // Генерация уникального идентификатора для доказательств
    string evidId = "EVID_" + to_string(time(nullptr));
    Evidence* evidence = new Evidence(evidId);

    // Создание копий кадров для независимого хранения доказательств
    // Это гарантирует, что оригинальные кадры могут быть удалены или изменены 
    // без потери доказательной базы
    for (auto frame : frames) {
        if (frame != nullptr) {  // ← ДОБАВИТЬ! Проверка на nullptr
            Frame* frameCopy = new Frame(frame->timestamp);
            evidence->addFrame(frameCopy);
        }
    }

    cout << "  [EvidenceCollector] Собрано доказательство " << evidId
        << " с " << frames.size() << " кадрами" << endl;

    return evidence;
}