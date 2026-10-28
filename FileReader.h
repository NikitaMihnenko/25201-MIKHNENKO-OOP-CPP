#pragma once

#include <fstream>
#include <string>

class FileReader {
public:
    explicit FileReader(const std::string& filePath);

    bool open();
    bool hasNext();
    std::string next();
    void close();
    void reset();

private:
    std::string filename;
    std::ifstream file;
};