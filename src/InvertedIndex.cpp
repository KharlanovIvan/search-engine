#include "InvertedIndex.h"



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

    // Цикл по всем документам для подсчета количества вхождений слова
    for (int index = 0; index < docs.size(); ++index) {
        Entry entry;
        entry.doc_id = index; // Устанавливаем идентификатор документа
        entry.count = 0;      // Инициализируем счетчик вхождений

        size_t pos = docs[index].find(word); // Ищем первое вхождение слова в документе

        // Пока находим вхождения слова в документе, увеличиваем счетчик
        while (pos != std::string::npos) {
            ++entry.count;
            pos = docs[index].find(word, pos + word.length()); // Ищем следующее вхождение
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
