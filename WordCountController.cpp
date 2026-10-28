#include "WordCountController.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "CsvWriter.h"
#include "FileReader.h"
#include "LineParser.h"
#include "WordStatistics.h"

namespace {

    std::string formatPercent(double value) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << value;
        return stream.str();
    }

}

WordCountController::WordCountController(const std::string& inputPath, const std::string& outputPath) : inputPath(inputPath), outputPath(outputPath) {
}

bool WordCountController::run() const {
    FileReader reader(inputPath);
    if (!reader.open()) {
        std::cerr << "Cannot open input file: " << inputPath << std::endl;
        return false;
    }

    LineParser parser;
    WordStatistics statistics;

    while (reader.hasNext()) {
        statistics.addWords(parser.parse(reader.next()));
    }
    reader.close();

    CsvWriter writer(outputPath);
    if (!writer.open()) {
        std::cerr << "Cannot open output file: " << outputPath << std::endl;
        return false;
    }

    writer.writeLine({"Word", "Count", "Frequency (%)"});

    const std::size_t total = statistics.getTotalCount();
    for (const WordStatistics::Entry& entry : statistics.getSortedByFrequency()) {
        double percent = entry.second * 100.0 / total;

        std::vector<std::string> row;
        row.push_back(entry.first);
        row.push_back(std::to_string(entry.second));
        row.push_back(formatPercent(percent));
        writer.writeLine(row);
    }
    writer.close();

    return true;
}