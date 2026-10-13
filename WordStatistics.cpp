#include "WordStatistics.h"

WordStatistics::WordStatistics() : totalCount(0) {
}

void WordStatistics::addWord(const std::string& word) {
    ++counts[word];
    ++totalCount;
}

void WordStatistics::addWords(const std::list<std::string>& words) {
    for (const std::string& word : words) {
        addWord(word);
    }
}

std::size_t WordStatistics::getTotalCount() const {
    return totalCount;
}

std::list<WordStatistics::Entry> WordStatistics::getSortedByFrequency() const {
    std::list<Entry> result(counts.begin(), counts.end());
    result.sort(compareEntries);
    return result;
}

bool WordStatistics::compareEntries(const Entry& a, const Entry& b) {
    if (a.second != b.second) {
        return a.second > b.second;
    }
    return a.first < b.first;
}