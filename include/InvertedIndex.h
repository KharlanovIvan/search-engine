#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <fstream>



struct Entry {
    size_t doc_id; 
    size_t count;

    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
    return (doc_id == other.doc_id &&
    count == other.count);
    }
};


class InvertedIndex {

    std::vector<std::string> docs; // Список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // Частотный словарь

public:
    InvertedIndex();

    void UpdateDocumentBase(std::vector<std::string> input_docs); // Обновить или заполнить базу документов, по которой будем совершать поиск

    std::vector<Entry> GetWordCount(const std::string& word); // Метод определяет количество вхождений слова word в загруженной базе документов

    const std::vector<std::string>& GetDocuments() const { // Метод для доступа к документам
        return docs;
    }

    const std::map<std::string, std::vector<Entry>>& GetFreqDictionary() const { // Метод для доступа к частотному словарю
        return freq_dictionary;
    }

     void PrintFreqDictionary() const {
        for (const auto& pair : freq_dictionary) {
            std::cout << "Word: " << pair.first << std::endl;
            for (const auto& entry : pair.second) {
                std::cout << "  Doc ID: " << entry.doc_id << ", Count: " << entry.count << std::endl;
            }
        }
    }
};
