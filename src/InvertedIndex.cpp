#include "InvertedIndex.h"



InvertedIndex::InvertedIndex(std::string configPath) 
    : docs(std::move(ConverterJSON::GetTextDocuments(configPath))) {};

/*
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
}
*/
    
/*
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
        std::vector<Entry> v;
        return v;

    }