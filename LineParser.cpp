#include "LineParser.h"

#include <cctype>

std::list<std::string> LineParser::parse(const std::string& line) const {
    std::list<std::string> words;
    std::string currentWord;

    for (std::size_t i = 0; i < line.size(); ++i) {
        unsigned char symbol = static_cast<unsigned char>(line[i]);

        if (std::isalnum(symbol)) {
            currentWord += static_cast<char>(std::tolower(symbol));
        } else if (!currentWord.empty()) {
            words.push_back(currentWord);
            currentWord.clear();
        }
    }

    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }

    return words;
}