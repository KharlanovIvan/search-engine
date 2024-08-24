#include "SearchServer.h"




// Конструктор, который инициализирует сервер поиска, принимая на вход объект обратного индекса
SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

// Метод для обработки запросов и поиска релевантных документов
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    // Вектор для хранения результатов поиска по всем запросам
    std::vector<std::vector<RelativeIndex>> sortListRelevantResponses;

    // Обработка каждого запроса из входного вектора
    for (const std::string& text : queries_input) {
        // Создаем поток ввода для обработки строки запроса
        std::istringstream iss(text);
        std::set<std::string> uniqueWordsSet;  // Множество для хранения уникальных слов из запроса
        std::string word;

        // Разбиваем строку на слова и добавляем их в множество (множество автоматически удаляет дубликаты)
        while (iss >> word) {
            uniqueWordsSet.insert(word);
        }

        // Преобразуем множество в вектор, чтобы можно было отсортировать слова
        std::vector<std::string> sortedRequestsWords(uniqueWordsSet.begin(), uniqueWordsSet.end());

        // Сортируем слова по возрастанию на основе их частоты встречаемости в документах
        std::sort(sortedRequestsWords.begin(), sortedRequestsWords.end(), [&](const std::string& word1, const std::string& word2) {
            size_t count1 = 0;
            size_t count2 = 0;

            // Получаем количество встречаемости первого слова
            if (_index.GetFreqDictionary().count(word1)) {
                for (const Entry& entry : _index.GetFreqDictionary().at(word1)) {
                    count1 += entry.count;
                }
            }

            // Получаем количество встречаемости второго слова
            if (_index.GetFreqDictionary().count(word2)) {
                for (const Entry& entry : _index.GetFreqDictionary().at(word2)) {
                    count2 += entry.count;
                }
            }
            return count1 < count2; // сортировка по возрастанию
        });

        // Создаем карту для хранения документов и их абсолютной релевантности
        std::map<size_t, size_t> doc_relevance;
        bool found = false;  // Флаг, указывающий на наличие найденных документов

        // Для каждого слова из запроса проверяем его наличие в частотном словаре
        for (const auto& word : sortedRequestsWords) {
            if (_index.GetFreqDictionary().count(word)) {
                found = true;  // Если хотя бы одно слово найдено, то документы точно есть
                // Если слово найдено, добавляем его в карту релевантности
                for (const auto& entry : _index.GetFreqDictionary().at(word)) {
                    doc_relevance[entry.doc_id] += entry.count;
                }
            }
        }

        // Если ни одно слово не найдено в словаре, пропускаем этот запрос
        if (!found) {
            sortListRelevantResponses.push_back({}); // добавляем пустой список в результат
            continue;  // Переходим к следующему запросу
        }

        // Определяем максимальную абсолютную релевантность для нормализации
        size_t max_relevance = 0;
        if (!doc_relevance.empty()) {
            max_relevance = std::max_element(doc_relevance.begin(), doc_relevance.end(),
                [](const auto& a, const auto& b) {
                    return a.second < b.second;
                })->second;
        }

        // Создаем вектор для хранения релевантных документов
        std::vector<RelativeIndex> relevant_docs;

        // Нормализуем релевантность и добавляем в вектор
        for (const auto& [doc_id, relevance] : doc_relevance) {
            float rank = static_cast<float>(relevance) / max_relevance;
            relevant_docs.push_back({doc_id, rank});
        }

        // Сортируем документы по убыванию релевантности
        std::sort(relevant_docs.begin(), relevant_docs.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            return a.rank > b.rank;
        });

        // Ограничиваем количество релевантных документов до максимального значения (max_responses)
        int max_responses = ConverterJSON::GetResponsesLimit();
        if (relevant_docs.size() > static_cast<size_t>(max_responses)) {
            relevant_docs.resize(max_responses);
        }

        // Добавляем результат для текущего запроса в итоговый список
        sortListRelevantResponses.push_back(relevant_docs);
    }

    // Возвращаем итоговый список релевантных документов для всех запросов
    return sortListRelevantResponses;
}
