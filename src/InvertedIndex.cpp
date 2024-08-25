#include "InvertedIndex.h"

#include <sstream>
#include <unordered_map>



// Конструктор по умолчанию для класса InvertedIndex
InvertedIndex::InvertedIndex() = default;

// Метод для обновления базы документов
void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;  // Сохраняем новые документы
    freq_dictionary.clear();  // Очищаем словарь частот при обновлении базы документов
}


// Метод для получения частот вхождения слова в документы
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    // Проверяем, есть ли слово в частотном словаре
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second; // Возвращаем результаты, если слово уже есть в словаре
    }

    std::vector<Entry> v; // Вектор для хранения результатов подсчета

    // Обходим все документы для подсчета количества вхождений слова
    for (int index = 0; index < docs.size(); ++index) {
        Entry entry;
        entry.doc_id = index; // Устанавливаем идентификатор документа
        entry.count = 0;      // Инициализируем счетчик вхождений

        // Создаем поток для разбора документа на слова
        std::istringstream stream(docs[index]);
        std::string token;

        // Читаем слова из документа
        while (stream >> token) {
            // Если слово совпадает с искомым, увеличиваем счетчик
            if (token == word) {
                ++entry.count;
            }
        }

        // Если слово было найдено хотя бы один раз, добавляем запись в вектор
        if (entry.count > 0) {
            v.push_back(entry);
        }
    }

    // Сохраняем результаты подсчета в словаре для ускорения последующих запросов
    freq_dictionary[word] = v;

    return v; // Возвращаем вектор с результатами
}


