#include "CsvWriter.h"

#include <fstream>
#include <iomanip>

CsvWriter::CsvWriter(const std::string& filePath) : filename(filePath) {
}

bool CsvWriter::write(const WordStatistics& statistics) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    const std::size_t total = statistics.getTotalCount();
    const std::list<WordStatistics::Entry> entries = statistics.getSortedByFrequency();

    file << "Word,Count,Frequency (%)\n";
    file << std::fixed << std::setprecision(2);

    for (const WordStatistics::Entry& entry : entries) {
        double percent = (total == 0) ? 0.0 : entry.second * 100.0 / total;
        file << entry.first << ',' << entry.second << ',' << percent << '\n';
    }

    return true;
}