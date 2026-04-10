#include "ResolutionGenerator.h"
#include "Resolution.h"    
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

Resolution* ResolutionGenerator::generate(Evidence* evidence, int vehicleSpeed, int recognitionConfidence) {
    int fineAmount = 500;

    if (vehicleSpeed > 80) {
        fineAmount = 1500;
    }
    else if (vehicleSpeed > 70) {
        fineAmount = 1000;
    }

    string resolutionId = "RES_" + to_string(time(nullptr));

    cout << "  [ResolutionGenerator] —формировано постановление: сумма штрафа="
        << fineAmount << " руб. (превышение до " << vehicleSpeed << " км/ч)" << endl;

    return new Resolution(resolutionId, fineAmount);
}