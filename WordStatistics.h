#pragma once

#include <cstddef>
#include <list>
#include <map>
#include <string>
#include <utility>

class WordStatistics {
public:
    typedef std::pair<std::string, std::size_t> Entry;

    WordStatistics();

    void addWord(const std::string& word);
    void addWords(const std::list<std::string>& words);

    std::size_t getTotalCount() const;
    std::list<Entry> getSortedByFrequency() const;

private:
    static bool compareEntries(const Entry& a, const Entry& b);

    std::map<std::string, std::size_t> counts;
    std::size_t totalCount;
};