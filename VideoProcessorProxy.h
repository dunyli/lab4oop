#pragma once
#include <queue>
#include <vector>
#include "VideoProcessor.h"

using namespace std;

// Класс-прокси для буферизации видеопотока
class VideoProcessorProxy {
public:
    queue<Frame*> buffer;        // Буфер для хранения кадров
    int maxSize;                  // Максимальный размер буфера
    VideoProcessor* realProcessor; // Реальный видеопроцессор

    // Конструктор и деструктор
    VideoProcessorProxy(VideoProcessor* p, int s);
    ~VideoProcessorProxy();

    // Методы
    void processFrame(Frame* frame, Vehicle* v, int quality, int confidence = 10);
    vector<Frame*> getAllFrames();
    int getBufferSize();
    void clearBuffer();
};