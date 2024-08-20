#include "ThreadPool.h"



// Конструктор, создающий потоки
ThreadPool::ThreadPool(size_t numThreads) : stop(false), activeTasks(0) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// Деструктор, завершающий все потоки
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

// Функция для добавления задач в очередь
void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(task);
        ++activeTasks;
    }
    condition.notify_one();
}

// Функция, выполняющая задачи из очереди
void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            --activeTasks; // Уменьшаем счетчик активных задач

            // Уведомляем, если все задачи выполнены
            if (activeTasks == 0) {
                condition.notify_all();
            }
        }
    }
}


void ThreadPool::waitAllTasks() {
    std::unique_lock<std::mutex> lock(queueMutex);
    condition.wait(lock, [this] { return tasks.empty() && activeTasks == 0; });
   
}