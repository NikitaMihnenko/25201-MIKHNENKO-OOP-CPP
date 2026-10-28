#include "CsvWriter.h"

CsvWriter::CsvWriter(const std::string& filePath) : filename(filePath) {
}

bool CsvWriter::open() {
    file.open(filename);
    return file.is_open();
}

void CsvWriter::writeLine(const std::vector<std::string>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            file << ',';
        }
        file << values[i];
    }
    file << '\n';
}

void CsvWriter::close() {
    if (file.is_open()) {
        file.close();
    }
}