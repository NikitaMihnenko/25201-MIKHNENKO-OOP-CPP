#include <iostream>

#include "WordCountController.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: word_count input.txt output.csv" << std::endl;
        return 1;
    }

    WordCountController controller(argv[1], argv[2]);

    return controller.run() ? 0 : 1;
}