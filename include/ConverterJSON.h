#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include "nlohmann/json.hpp"


class ConverterJSON {

public:
    ConverterJSON();

    // Метод получения содержимого файлов возвращает список с содержимым файлов перечисленных в config.json 
    static std::vector<std::string> GetTextDocuments();

    // Метод считывает поле max_responses для определения предельного количества ответов на один запрос 
    static int GetResponsesLimit();

    // Метод получения запросов из файла requests.json возвращает список запросов из файла requests.json 
    static std::vector<std::string> GetRequests();

    // Положить в файл answers.json результаты поисковых запросов
    static void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};