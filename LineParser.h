#pragma once

#include <list>
#include <string>

class LineParser {
public:
    std::list<std::string> parse(const std::string& line) const;
};