#include <iostream>
#include <vector>
#include <fstream>

#include "ConverterJSON.h"
#include "InvertedIndex.h"



int main() {
    
    std::vector<std::string> fileContetnts;

    fileContetnts = ConverterJSON::GetTextDocuments("../../config.json");

    for(std::string fileText : fileContetnts) {
        std::cout << fileText << std::endl;
        std::cout << "_____________________________________________________________________" << std::endl;
    }

    int mR = ConverterJSON::GetResponsesLimit("../../config.json");
    std::cout << "MAX_RESPONSES = " << mR << std::endl;

    std::vector<std::string> requests = ConverterJSON::GetRequests("../../requests.json");
    int i = 0;
    for(std::string request : requests) {
        std::cout << "REQUEST #" << ++i << ": " << request <<std::endl; 
    }

    return 0;
}

