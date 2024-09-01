#include "ConverterJSON.h"



const std::string VERSION = "0.1";
const std::string CONFIG_PATH = "config.json";  // Путь к конфигурационному файлу
const std::string REQUESTS_PATH = "requests.json"; // Путь к файлу запросов
const std::string ANSWERS_PATH = "answers.json";  // Путь к файлу ответов


ConverterJSON::ConverterJSON() = default; // Конструктор по умолчанию


// Метод для получения содержимого текстовых документов из файлов, указанных в конфигурационном файле
std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> fileContents; // Вектор для хранения содержимого файлов

    // Открываем конфигурационный файл
    std::ifstream configFile(CONFIG_PATH);
    if (!configFile.is_open()) {
        throw std::runtime_error("Config file is missing."); // Выбрасываем исключение, если файл не найден
    }

    nlohmann::json config;
    configFile >> config; // Загружаем содержимое конфигурационного файла в объект JSON

    // Проверка целостности конфигурационного файла
    if (!config.contains("config")) {
        throw std::runtime_error("Config file is corrupted: 'config' section is missing.");
    }

    auto& configSection = config.at("config");
    if (!configSection.contains("name") || !configSection.contains("version") || !configSection.contains("max_responses")) {
        throw std::runtime_error("Config file is corrupted: 'name', 'version', or 'max_responses' fields are missing.");
    }

    if (config.at("config").at("version") != VERSION) {
        throw std::runtime_error("Config.json file has incorrect file version");
    }

    // Получение списка путей к файлам из конфигурационного файла
    std::vector<std::string> filePaths = config.at("files").get<std::vector<std::string>>();

    // Чтение содержимого каждого файла и сбор в список
    for (const auto& filePath : filePaths) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening the file: " << filePath << std::endl; // Сообщаем об ошибке открытия файла
            continue;
        }

        std::stringstream buffer;
        buffer << file.rdbuf(); // Чтение всего содержимого файла в буфер
        fileContents.push_back(buffer.str()); // Добавляем содержимое файла в вектор
    }

    std::cout << "Starting..." << std::endl;
    std::cout << config.at("config").at("name").get<std::string>()
              << " version " << config.at("config").at("version").get<std::string>() << std::endl;

    return fileContents; // Возвращаем вектор с содержимым файлов
}

// Метод для получения предельного количества ответов на запрос из конфигурационного файла
int ConverterJSON::GetResponsesLimit() {
    int maxResponses;

    // Открываем конфигурационный файл
    std::ifstream configFile(CONFIG_PATH);
    if (!configFile.is_open()) {
        throw std::runtime_error("Config file is missing."); // Выбрасываем исключение, если файл не найден
    }

    nlohmann::json config;
    configFile >> config; // Загружаем содержимое конфигурационного файла в объект JSON

    // Проверка целостности конфигурационного файла
    if (!config.contains("config")) {
        throw std::runtime_error("Config file is corrupted: 'config' section is missing.");
    }

    auto& configSection = config.at("config");
    if (!configSection.contains("name") || !configSection.contains("version") || !configSection.contains("max_responses")) {
        throw std::runtime_error("Config file is corrupted: 'name', 'version', or 'max_responses' fields are missing.");
    }

    maxResponses = config.at("config").at("max_responses").get<int>(); // Получаем значение max_responses из конфигурационного файла

    return maxResponses; // Возвращаем значение max_responses
}

// Метод для получения запросов из файла запросов
std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requestsList;

    // Открываем файл запросов
    std::ifstream requestsFile(REQUESTS_PATH);
    if (!requestsFile.is_open()) {
        throw std::runtime_error("The request file could not be opened."); // Выбрасываем исключение, если файл не найден
    }

    nlohmann::json requests;
    requestsFile >> requests; // Загружаем содержимое файла запросов в объект JSON
    requestsList = requests.at("requests").get<std::vector<std::string>>(); // Получаем список запросов из JSON

    return requestsList; // Возвращаем список запросов
}

// Метод для записи результатов поиска в файл answers.json
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    nlohmann::json j;

    // Проходим по всем запросам и собираем результаты
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string request_id = "request" + std::to_string(i + 1);
        nlohmann::json request_json;

        if (!answers[i].empty()) {
            request_json["result"] = "true"; // Если есть релевантные документы

            nlohmann::json relevance_json = nlohmann::json::array();
            for (const auto& entry : answers[i]) {
                nlohmann::json doc_json;
                doc_json["docid"] = entry.first;  // Используем первый элемент пары как идентификатор документа (doc_id)
                doc_json["rank"] = entry.second; // Используем второй элемент пары как ранг документа (rank)
                relevance_json.push_back(doc_json); // Добавляем документ в массив релевантности
            }

            request_json["relevance"] = relevance_json; // Добавляем массив релевантности в результат
        } else {
            request_json["result"] = "false"; // Если ни одного релевантного документа нет
        }

        j["answers"][request_id] = request_json; // Добавляем результат для текущего запроса в итоговый JSON
    }

    // Записываем итоговый JSON в файл
    std::ofstream out(ANSWERS_PATH);
    out << j.dump(4); // 4 - задает отступы для читабельности JSON-файла
    out.close();
}

