#include <iostream>
#include <vector>
#include <thread>

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "ThreadPool.h"
#include "SearchServer.h"



int main() {

    try {
        // Создание инвертированного индекса
        InvertedIndex invertedIndex;
        
        try {
            // Обновление базы данных документов из конфигурационного файла
            invertedIndex.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
        } catch (const nlohmann::json::exception& e) {
            // Обработка исключений, связанных с JSON (ошибки парсинга или доступа к данным)
            std::cerr << e.what() << std::endl;
            return 1;
        } catch (const std::runtime_error& e) {
            // Обработка других ошибок, например, отсутствия конфигурационного файла
            std::cerr << e.what() << std::endl;
            return 1;
        }
        
        {
            // Создание пула потоков с количеством потоков, равным количеству ядер процессора
            ThreadPool threadPool(std::thread::hardware_concurrency());

            // Обработка каждого документа в отдельном потоке
            for (int i = 0; i < invertedIndex.GetDocuments().size(); ++i) {
                // lambda-функция для выполнения задачи в потоке
                threadPool.enqueueTask([&invertedIndex, i] {
                    std::istringstream iss(invertedIndex.GetDocuments()[i]);
                    std::string word;
                    while (iss >> word) {
                        invertedIndex.GetWordCount(word); // Индексация слов
                    }
                });
            }

            // Ожидание завершения всех задач в пуле потоков
            threadPool.waitAllTasks();
            // Когда все задачи завершены, потоки автоматически завершат работу при разрушении threadPool
        }
        
        try {
            // Создание сервера поиска на основе инвертированного индекса
            SearchServer searchServer(invertedIndex);
            std::vector<std::vector<std::pair<int, float>>> convertedAnswers;

            // Проходим по каждому запросу и его результатам
            for (const auto& result : searchServer.search(ConverterJSON::GetRequests())) {
                std::vector<std::pair<int, float>> convertedResult;
                for (const auto& relIndex : result) {
                    // Преобразуем RelativeIndex в std::pair<int, float>. Не знаю зачем, но метод putAnswers в техзадании принимал именно этот тип.
                    convertedResult.emplace_back(static_cast<int>(relIndex.doc_id), relIndex.rank);
                }
                convertedAnswers.push_back(std::move(convertedResult));
            }
            // Передаем преобразованные данные в метод putAnswers
            ConverterJSON::putAnswers(convertedAnswers);
            
        } catch (const nlohmann::json::exception& e) {
            // Обработка исключений, связанных с JSON (ошибки парсинга или доступа к данным)
            std::cerr << e.what() << std::endl;
            return 1;
        } catch (const std::runtime_error& e) {
            // Обработка других ошибок, например, отсутствия файла с запросами
            std::cerr << e.what() << std::endl;
            return 1;
        }
        
        // Успешное завершение программы
        std::cout << "The program is completed." << std::endl;
    } catch (const std::exception& e) {
        // Обработка любых других неизвестных исключений
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return 1; 
    }

    return 0;
}