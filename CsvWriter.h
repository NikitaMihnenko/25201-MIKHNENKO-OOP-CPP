#pragma once

#include <string>
#include "WordStatistics.h"

class CsvWriter {
public:
    explicit CsvWriter(const std::string& filePath);

    bool write(const WordStatistics& statistics) const;

private:
    std::string filename;
};