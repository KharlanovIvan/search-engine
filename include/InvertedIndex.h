#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>



// Структура для хранения информации о вхождении слова в документ
struct Entry {
    size_t doc_id; // Идентификатор документа
    size_t count;  // Количество вхождений слова в документе

    // Оператор сравнения для структуры Entry, используемый для тестирования
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

// Класс для реализации инвертированного индекса
class InvertedIndex {
    std::vector<std::string> docs; // Вектор строк, содержащий текст документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // Словарь, хранящий вектор Entry для каждого слова

public:
    // Конструктор по умолчанию
    InvertedIndex();

    // Метод для обновления или заполнения базы документов
    // input_docs - вектор строк, где каждая строка представляет собой документ
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    // Метод для определения количества вхождений слова word в загруженной базе документов
    // word - слово, для которого нужно подсчитать вхождения
    std::vector<Entry> GetWordCount(const std::string& word);

    // Метод для доступа к документам
    // Возвращает константную ссылку на вектор документов
    const std::vector<std::string>& GetDocuments() const { 
        return docs; 
    }

    // Метод для доступа к частотному словарю
    // Возвращает константную ссылку на частотный словарь
    const std::map<std::string, std::vector<Entry>>& GetFreqDictionary() const { 
        return freq_dictionary; 
    }
};
