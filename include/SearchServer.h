#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <sstream>

#include "InvertedIndex.h"
#include "ConverterJSON.h"


// Структура для хранения относительного индекса документа
struct RelativeIndex {
    size_t doc_id;  // Идентификатор документа
    float rank;     // Релевантность документа

    // Оператор сравнения для проверки эквивалентности двух объектов RelativeIndex
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};


// Класс для реализации логики поиска по проиндексированным данным
class SearchServer {

    // Обратный индекс, используемый для поиска релевантных документов
    InvertedIndex _index;

public:
    // Конструктор, принимающий ссылку на объект обратного индекса
    SearchServer(InvertedIndex& idx);

    // Метод для обработки поисковых запросов и получения релевантных документов
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};
