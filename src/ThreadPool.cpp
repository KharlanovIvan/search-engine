#include "ThreadPool.h"



// Конструктор, создающий потоки
// numThreads - количество потоков, которые будут работать в пуле
ThreadPool::ThreadPool(size_t numThreads) : stop(false), activeTasks(0) {
    // Создаем потоки и добавляем их в вектор workers
    for (size_t i = 0; i < numThreads; ++i) {
        // Каждый поток выполняет функцию workerThread, которая будет вытаскивать задачи из очереди и выполнять их
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// Деструктор, завершающий все потоки
ThreadPool::~ThreadPool() {
    {
        // Захватываем мьютекс, чтобы безопасно изменить флаг stop
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true; // Устанавливаем флаг завершения работы потоков
    }
    // Уведомляем все потоки, чтобы они прекратили ожидание и завершили свою работу
    condition.notify_all();
    // Присоединяемся к каждому потоку, чтобы дождаться его завершения
    for (std::thread &worker : workers) {
        worker.join();
    }
}

// Метод для добавления задач в очередь
// task - задача, которую необходимо выполнить
void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        // Захватываем мьютекс для безопасного добавления задачи в очередь
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(task); // Добавляем задачу в очередь
        ++activeTasks; // Увеличиваем счетчик активных задач
    }
    // Уведомляем один поток, который может взять задачу из очереди
    condition.notify_one();
}

// Метод, выполняющий задачи из очереди
void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            // Захватываем мьютекс, чтобы безопасно получить задачу из очереди
            std::unique_lock<std::mutex> lock(queueMutex);
            // Ждем, пока не будет задач в очереди или не придет сигнал на завершение работы
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            // Если установлен флаг stop и задач в очереди нет, завершаем работу потока
            if (stop && tasks.empty()) {
                return;
            }
            // Извлекаем задачу из очереди
            task = std::move(tasks.front());
            tasks.pop();
        }
        // Выполняем задачу вне блокировки, чтобы не удерживать мьютекс во время выполнения
        task();
        {
            // Захватываем мьютекс, чтобы безопасно уменьшить счетчик активных задач
            std::unique_lock<std::mutex> lock(queueMutex);
            --activeTasks; // Уменьшаем счетчик активных задач

            // Уведомляем все потоки, если все задачи выполнены
            if (activeTasks == 0) {
                condition.notify_all();
            }
        }
    }
}

// Метод ожидания завершения всех задач
void ThreadPool::waitAllTasks() {
    std::unique_lock<std::mutex> lock(queueMutex);
    // Ждем, пока очередь задач не будет пуста и не завершатся все активные задачи
    condition.wait(lock, [this] { return tasks.empty() && activeTasks == 0; });
}
