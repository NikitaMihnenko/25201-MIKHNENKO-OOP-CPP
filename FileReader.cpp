#include "FileReader.h"

FileReader::FileReader(const std::string& filePath) : filename(filePath) {
}

bool FileReader::open() {
    file.open(filename);
    return file.is_open();
}

bool FileReader::hasNext() {
    return file.is_open() && file.peek() != std::ifstream::traits_type::eof();
}

std::string FileReader::next() {
    std::string line;
    if (hasNext()) {
        std::getline(file, line);
    }
    return line;
}

void FileReader::close() {
    if (file.is_open()) {
        file.close();
    }
}

void FileReader::reset() {
    file.clear();
    file.seekg(0, std::ios::beg);
}