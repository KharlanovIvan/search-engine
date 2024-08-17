#include <iostream>
#include <vector>
#include <fstream>

#include "nlohmann/json.hpp"







class InvertedIndex {

    public:

};




/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {

public:
    ConverterJSON() = default;


/* Метод получения содержимого файлов возвращает список с содержимым файлов перечисленных в config.json */
static std::vector<std::string> GetTextDocuments() {
    std::string configPath = "../config.json";
    std::vector<std::string> fileContents;

    try {
        // Чтение конфигурационного файла
        std::ifstream configFile(configPath);
        if (!configFile.is_open()) {
            throw std::runtime_error("Не удалось открыть конфигурационный файл");
        } else { 
            std::cerr << "Конфигурационный файл открыт" << std::endl;
        }
    
        nlohmann::json config;
        configFile >> config;

        // Получение списка путей к файлам из конфигурационного файла
        std::vector<std::string> filePaths = config.at("files").get<std::vector<std::string>>();

        // Чтение содержимого каждого файла и сбор в список
        for (const auto& filePath : filePaths) {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                std::cerr << "Ошибка при открытии файла " << filePath << std::endl;
                continue;
            }

            std::stringstream buffer;
            buffer << file.rdbuf(); // Чтение всего содержимого файла в buffer
            fileContents.push_back(buffer.str());
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Ошибка при обработке конфигурационного файла: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    // Возвращение списка содержимого файлов
    return fileContents;
}

/* Метод считывает поле max_responses для определения предельного количества ответов на один запрос */
static int GetResponsesLimit() {
    int maxResponses;
    std::string configPath = "../config.json";
    try {
        std::ifstream configFile(configPath);
        if(!configFile.is_open()) {
            throw std::runtime_error("Не удалось открыть конфигурационный файл");
        } else { 
            std::cerr << "Конфигурационный файл открыт" << std::endl;
        }

        nlohmann::json config;
        configFile >> config;

        maxResponses = config.at("config").at("max_responses").get<int>();

    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Ошибка при обработке конфигурационного файла: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return maxResponses;
}
/* Метод получения запросов из файла requests.json возвращает список запросов из файла requests.json */
static std::vector<std::string> GetRequests() {
    std::vector<std::string> requestsList;
    std::string requestsPath = "../requests.json";
    try {
        std::ifstream requestsFile(requestsPath);
        if (!requestsFile.is_open()) {
            throw std::runtime_error("Не удалось открыть файл с запросами");
        } else { 
            std::cerr << "Файл с запросами открыт" << std::endl;
        }

        nlohmann::json requests;
        requestsFile >> requests;
        requestsList = requests.at("requests").get<std::vector<std::string>>();

    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Ошибка при обработке файла с запросами: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return requestsList;
}



/* Положить в файл answers.json результаты поисковых запросов */
void putAnswers(std::vector<std::vector<std::pair<int, float>>>
answers);

};





int main() {
    
std::vector<std::string> fileContetnts;

fileContetnts = ConverterJSON::GetTextDocuments();

for(std::string fileText : fileContetnts) {
    std::cout << fileText << std::endl;
    std::cout << "_____________________________________________________________________" << std::endl;
}
int mR = ConverterJSON::GetResponsesLimit();
std::cout << "MAX_RESPONSES = " << mR;

std::vector<std::string> requests = ConverterJSON::GetRequests();
int i = 0;
for(std::string request : requests) {
    std::cout << "REQUEST #" << ++i << ": " <<request <<std::endl; 
}

    return 0;
}

