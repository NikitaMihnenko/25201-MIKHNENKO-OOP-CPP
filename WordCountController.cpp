#include "WordCountController.h"

#include <iostream>

#include "CsvWriter.h"
#include "FileReader.h"
#include "LineParser.h"
#include "WordStatistics.h"

WordCountController::WordCountController(const std::string& inputPath, const std::string& outputPath) : inputPath(inputPath), outputPath(outputPath) {
}

bool WordCountController::run() const {
    FileReader reader(inputPath);
    if (!reader.load()) {
        std::cerr << "Cannot open input file: " << inputPath << std::endl;
        return false;
    }

    LineParser parser;
    WordStatistics statistics;

    for (const std::string& line : reader.getLines()) {
        statistics.addWords(parser.parse(line));
    }

    CsvWriter writer(outputPath);
    if (!writer.write(statistics)) {
        std::cerr << "Cannot write output file: " << outputPath << std::endl;
        return false;
    }

    return true;
}