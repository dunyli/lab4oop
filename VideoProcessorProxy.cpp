#include "VideoProcessorProxy.h"
#include <iostream>

using namespace std;

// Конструктор
VideoProcessorProxy::VideoProcessorProxy(VideoProcessor* p, int s)
    : realProcessor(p), maxSize(s) {
    cout << "[VideoProcessorProxy] Создан с размером буфера: " << maxSize << " кадров" << endl;
}

// Деструктор - очищаем буфер
VideoProcessorProxy::~VideoProcessorProxy() {
    cout << "[VideoProcessorProxy] Очистка буфера, удаляется " << buffer.size() << " кадров" << endl;
    while (!buffer.empty()) {
        delete buffer.front();
        buffer.pop();
    }
}

// Обработка кадра через прокси
void VideoProcessorProxy::processFrame(Frame* frame, Vehicle* v, int quality, int confidence) {
    // 1. ПРОКСИ: сохраняем кадр в буфер
    buffer.push(frame);

    // Если буфер переполнен, удаляем самый старый кадр
    if (buffer.size() > maxSize) {
        delete buffer.front();
        buffer.pop();
    }

    cout << "[VideoProcessorProxy] Кадр добавлен в буфер. Размер буфера: " << buffer.size()
        << "/" << maxSize << endl;

    // 2. ДЕЛЕГИРОВАНИЕ: передаем управление реальному процессору
    realProcessor->processFrame(frame, v, quality, confidence);
}

// Получить все кадры из буфера (для доказательств)
vector<Frame*> VideoProcessorProxy::getAllFrames() {
    vector<Frame*> frames;
    queue<Frame*> temp = buffer;

    cout << "[VideoProcessorProxy] Запрос всех кадров из буфера. Размер: " << buffer.size() << endl;

    // Копируем кадры из очереди в вектор
    while (!temp.empty()) {
        frames.push_back(temp.front());
        temp.pop();
    }

    cout << "[VideoProcessorProxy] Получено " << frames.size() << " кадров из буфера" << endl;
    return frames;
}

// Получить размер буфера
int VideoProcessorProxy::getBufferSize() {
    return buffer.size();
}

// Очистить буфер
void VideoProcessorProxy::clearBuffer() {
    cout << "[VideoProcessorProxy] Очистка буфера" << endl;
    while (!buffer.empty()) {
        delete buffer.front();
        buffer.pop();
    }
    cout << "[VideoProcessorProxy] Буфер очищен" << endl;
}