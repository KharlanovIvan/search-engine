#include "InvertedIndex.h"



InvertedIndex::InvertedIndex() = default;


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    freq_dictionary.clear();  // Очистка словаря при обновлении базы документов
}

    
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    // Проверка на существование слова в словаре
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second; // Возвращаем результат, если слово уже есть в словаре
    }

    std::vector<Entry> v;

    // Цикл по всем документам для подсчета вхождений слова
    for(int index = 0; index < docs.size(); ++index) {
        Entry entry;
        entry.doc_id = index;
        entry.count = 0;

        size_t pos = docs[index].find(word);

        while (pos != std::string::npos) {
            ++entry.count;
            pos = docs[index].find(word, pos + word.length()); 
        }

        // Если слово было найдено хотя бы один раз, добавляем его в вектор
        if (entry.count > 0) {
            v.push_back(entry);
        }
    }

    // Сохраняем результаты подсчета в словаре для последующего использования
    freq_dictionary[word] = v;

    return v;
}

