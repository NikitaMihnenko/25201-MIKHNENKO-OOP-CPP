#pragma once

#include <string>

class WordCountController {
public:
    WordCountController(const std::string& inputPath, const std::string& outputPath);

    bool run() const;

private:
    std::string inputPath;
    std::string outputPath;
};