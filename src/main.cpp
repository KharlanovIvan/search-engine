#include <iostream>
#include <vector>
#include <fstream>
#include<thread>

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "ThreadPool.h"

int main() {
    std::string configPath = "../../config.json";
    std::string requestsPath = "../../requests.json";

    
    
    // Создание инвертированного индекса
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(ConverterJSON::GetTextDocuments(configPath));
    
    {
        // Создание пула потоков с количеством потоков, равным количеству процессорных ядер
        ThreadPool threadPool(std::thread::hardware_concurrency());

        std::cout << "hardware_concurrency = " << std::thread::hardware_concurrency() << std::endl;

        // Обработка каждого документа в отдельном потоке
        for (int i = 0; i < invertedIndex.GetDocuments().size(); ++i) {
            std::cout << "Цикл распределения индексации фалов по потокам запущен!" << std::endl;
            // lambda-функция для выполнения задачи в потоке
            threadPool.enqueueTask([&invertedIndex, i] {
                std::istringstream iss(invertedIndex.GetDocuments()[i]);
                std::string word;
                while (iss >> word) {
                    invertedIndex.GetWordCount(word); // Индексация слов
                }
            });
        }

        threadPool.waitAllTasks();
        // Когда все задачи завершены, потоки автоматически завершат работу при разрушении threadPool
    }
    

    std::cout << "Программа завершена." << std::endl;
    return 0;
}
