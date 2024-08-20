#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>



/***************************************************************************************************************************************************
    Преимущества использования пула потоков:
1. Ограничение количества одновременно выполняемых потоков, что предотвращает перегрузку системы.
2. Уменьшение накладных расходов на создание и уничтожение потоков, так как потоки создаются один раз при запуске программы.
3. Эффективное использование процессорных ядер, так как пул потоков можно настроить на использование оптимального количества потоков.

Использование пула потоков позволяет лучше контролировать использование ресурсов и избежать потенциальных проблем, 
связанных с созданием большого количества потоков. 
В нашем случае, когда количество файлов неизвестно заранее, пул потоков будет распределять задачи индексации файлов по ограниченному числу потоков, 
обеспечивая эффективную обработку файлов без перегрузки процессора.
****************************************************************************************************************************************************/

// Класс для управления пулом потоков
class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;

    bool stop;

    void workerThread(); // Потоки постоянно проверяют, есть ли новые задачи в очереди, и выполняют их, когда они появляются.

    std::atomic<size_t> activeTasks; // Счетчик активных задач

public:
    // 
    ThreadPool(size_t numThreads); /* создает фиксированное количество потоков (workers), которые постоянно работают в фоне и ждут задач из очереди.
                                      Задачи помещаются в очередь с помощью метода enqueueTask.
                                      Потоки в пуле вытягивают задачи из очереди и выполняют их.*/

    ~ThreadPool();

    void enqueueTask(std::function<void()> task);

    void waitAllTasks();  // Метод для ожидания завершения всех задач
};
