#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <iterator>
#include <list>
#include <string>

#include "CsvWriter.h"
#include "FileReader.h"
#include "LineParser.h"
#include "WordCountController.h"
#include "WordStatistics.h"

namespace {

typedef std::list<std::string> Words;

void writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

}

TEST(LineParserTest, SplitsByDelimitersAndLowercases) {
    LineParser parser;
    EXPECT_EQ(parser.parse("Hello, WORLD! abc123"), (Words{"hello", "world", "abc123"}));
}

TEST(LineParserTest, EmptyLineGivesNoWords) {
    LineParser parser;
    EXPECT_TRUE(parser.parse(" ,.! ").empty());
}


TEST(WordStatisticsTest, CountsAndSortsByFrequency) {
    WordStatistics stats;
    stats.addWords(Words{"rare", "common", "common"});

    EXPECT_EQ(stats.getTotalCount(), 3u);

    std::list<WordStatistics::Entry> entries = stats.getSortedByFrequency();
    ASSERT_EQ(entries.size(), 2u);
    EXPECT_EQ(entries.front().first, "common");
    EXPECT_EQ(entries.front().second, 2u);
    EXPECT_EQ(entries.back().first, "rare");
}

TEST(WordStatisticsTest, EqualFrequenciesSortedAlphabetically) {
    WordStatistics stats;
    stats.addWords(Words{"cat", "ant"});

    EXPECT_EQ(stats.getSortedByFrequency().front().first, "ant");
}

TEST(FileReaderTest, ReadsAllLines) {
    writeFile("fr_test.txt", "first\nsecond");

    FileReader reader("fr_test.txt");
    ASSERT_TRUE(reader.load());
    EXPECT_EQ(reader.getLines(), (Words{"first", "second"}));

    std::remove("fr_test.txt");
}

TEST(FileReaderTest, MissingFileFails) {
    FileReader reader("this_file_does_not_exist.txt");
    EXPECT_FALSE(reader.load());
}

TEST(CsvWriterTest, WritesHeaderRowsAndPercentages) {
    WordStatistics stats;
    stats.addWords(Words{"apple", "apple", "banana"});

    CsvWriter writer("csv_test.csv");
    ASSERT_TRUE(writer.write(stats));

    EXPECT_EQ(readFile("csv_test.csv"), "Word,Count,Frequency (%)\n" "apple,2,66.67\n" "banana,1,33.33\n");

    std::remove("csv_test.csv");
}

TEST(CsvWriterTest, EmptyStatisticsWritesOnlyHeader) {
    WordStatistics stats;

    CsvWriter writer("csv_empty.csv");
    ASSERT_TRUE(writer.write(stats));

    EXPECT_EQ(readFile("csv_empty.csv"), "Word,Count,Frequency (%)\n");

    std::remove("csv_empty.csv");
}

TEST(WordCountControllerTest, CountsWordsEndToEnd) {
    writeFile("ctrl_in.txt", "The cat, the dog.\nThe end");

    WordCountController controller("ctrl_in.txt", "ctrl_out.csv");
    ASSERT_TRUE(controller.run());

    EXPECT_EQ(readFile("ctrl_out.csv"),"Word,Count,Frequency (%)\n" "the,3,50.00\n" "cat,1,16.67\n" "dog,1,16.67\n" "end,1,16.67\n");

    std::remove("ctrl_in.txt");
    std::remove("ctrl_out.csv");
}

TEST(WordCountControllerTest, MissingInputFileFails) {
    WordCountController controller("nonexistent_input.txt", "never_created.csv");
    EXPECT_FALSE(controller.run());
}