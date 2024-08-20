#include "ConverterJSON.h"



ConverterJSON::ConverterJSON() = default;


std::vector<std::string> ConverterJSON::GetTextDocuments(const std::string configPath) {
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


int ConverterJSON::GetResponsesLimit(const std::string configPath) {
    int maxResponses;
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



std::vector<std::string> ConverterJSON::GetRequests(const std::string requestsPath) {
    std::vector<std::string> requestsList;
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


void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>
answers){
    
}




