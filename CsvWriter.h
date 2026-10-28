#pragma once

#include <fstream>
#include <string>
#include <vector>

class CsvWriter {
public:
    explicit CsvWriter(const std::string& filePath);

    bool open();
    void writeLine(const std::vector<std::string>& values);
    void close();

private:
    std::string filename;
    std::ofstream file;
};