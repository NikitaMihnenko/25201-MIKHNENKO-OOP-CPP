#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../ConvertToCSV.h"

using namespace std;

class ConvertToCSVTest : public ::testing::Test {
protected:
    const string inputFile = "test_input.txt";
    const string outputFile = "test_output.csv";

    void TearDown() override {
        remove(inputFile.c_str());
        remove(outputFile.c_str());
    }

    void createInputFile(const string& content) {
        ofstream file(inputFile);
        file << content;
        file.close();
    }

    string readOutputFile() {
        ifstream file(outputFile);
        return string(
            (istreambuf_iterator<char>(file)),
            istreambuf_iterator<char>()
        );
    }
};

TEST_F(ConvertToCSVTest, CountsWords) {
    createInputFile("apple apple banana");

    ConvertToCSV(inputFile, outputFile);

    string result = readOutputFile();

    EXPECT_NE(result.find("apple,2,66.67"), string::npos);
    EXPECT_NE(result.find("banana,1,33.33"), string::npos);
}

TEST_F(ConvertToCSVTest, HandlesPunctuation) {
    createInputFile("hello, hello! world.");

    ConvertToCSV(inputFile, outputFile);

    string result = readOutputFile();

    EXPECT_NE(result.find("hello,2,66.67"), string::npos);
    EXPECT_NE(result.find("world,1,33.33"), string::npos);
}

TEST_F(ConvertToCSVTest, CreatesCSVHeader) {
    createInputFile("hello world");

    ConvertToCSV(inputFile, outputFile);

    string result = readOutputFile();

    EXPECT_EQ(
    result.compare(0, std::string("word,frequency,frequency%\n").size(),
                   "word,frequency,frequency%\n"),
    0
);
}

TEST_F(ConvertToCSVTest, HandlesEmptyFile) {
    createInputFile("");

    ConvertToCSV(inputFile, outputFile);

    string result = readOutputFile();

    EXPECT_EQ(result, "word,frequency,frequency%\n");
}

TEST_F(ConvertToCSVTest, HandlesMissingInputFile) {
    ConvertToCSV("nonexistent.txt", outputFile);

    ifstream file(outputFile);

    EXPECT_FALSE(file.is_open());
}