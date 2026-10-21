#include "FileReader.h"

FileReader::FileReader(const std::string& filePath) : filename(filePath), loaded(false) {
}


bool FileReader::load() {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    lines.clear();

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    loaded = true;
    return true;
}

const std::list<std::string>& FileReader::getLines() const {
    return lines;
}

bool FileReader::isLoaded() const {
    return loaded;
}