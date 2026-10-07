#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <list>

class FileReader {
private:
    std::string filename;
    std::list<std::string> lines;
    bool loaded;

public:
    explicit FileReader(const std::string& filePath);

    bool load();

    const std::list<std::string>& getLines() const;

    bool isLoaded() const;
};

#endif