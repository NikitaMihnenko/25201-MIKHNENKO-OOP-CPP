#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

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
    EXPECT_EQ(parser.parse("Hello, WORLD! abc123"),
              (Words{"hello", "world", "abc123"}));
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
    EXPECT_EQ(entries.back().second, 1u);
}

TEST(WordStatisticsTest, EqualFrequenciesSortedAlphabetically) {
    WordStatistics stats;
    stats.addWords(Words{"cat", "ant"});

    EXPECT_EQ(stats.getSortedByFrequency().front().first, "ant");
}

TEST(FileReaderTest, ReadsLinesOneByOne) {
    writeFile("fr_test.txt", "first\nsecond");

    FileReader reader("fr_test.txt");
    reader.open();

    ASSERT_TRUE(reader.hasNext());
    EXPECT_EQ(reader.next(), "first");
    ASSERT_TRUE(reader.hasNext());
    EXPECT_EQ(reader.next(), "second");
    EXPECT_FALSE(reader.hasNext());

    reader.close();
    std::remove("fr_test.txt");
}

TEST(FileReaderTest, ResetReturnsToBeginning) {
    writeFile("fr_reset.txt", "first\nsecond");

    FileReader reader("fr_reset.txt");
    reader.open();
    reader.next();
    reader.next();
    ASSERT_FALSE(reader.hasNext());

    reader.reset();
    ASSERT_TRUE(reader.hasNext());
    EXPECT_EQ(reader.next(), "first");

    reader.close();
    std::remove("fr_reset.txt");
}

TEST(FileReaderTest, OpenMissingFileThrows) {
    FileReader reader("this_file_does_not_exist.txt");
    EXPECT_THROW(reader.open(), std::runtime_error);
}

TEST(CsvWriterTest, WritesValuesSeparatedByCommas) {
    CsvWriter writer("csv_test.csv");
    writer.open();
    writer.writeLine({"a", "b", "c"});
    writer.writeLine({"x"});
    writer.close();

    EXPECT_EQ(readFile("csv_test.csv"), "a,b,c\nx\n");

    std::remove("csv_test.csv");
}

TEST(CsvWriterTest, OpenInMissingDirectoryThrows) {
    CsvWriter writer("no_such_directory/out.csv");
    EXPECT_THROW(writer.open(), std::runtime_error);
}

TEST(WordCountControllerTest, CountsWordsEndToEnd) {
    writeFile("ctrl_in.txt", "The cat, the dog.\nThe end");

    WordCountController controller("ctrl_in.txt", "ctrl_out.csv");
    ASSERT_TRUE(controller.run());

    EXPECT_EQ(readFile("ctrl_out.csv"),
              "Word,Count,Frequency (%)\n"
              "the,3,50.00\n"
              "cat,1,16.67\n"
              "dog,1,16.67\n"
              "end,1,16.67\n");

    std::remove("ctrl_in.txt");
    std::remove("ctrl_out.csv");
}

TEST(WordCountControllerTest, EmptyInputWritesOnlyHeader) {
    writeFile("ctrl_empty_in.txt", "");

    WordCountController controller("ctrl_empty_in.txt", "ctrl_empty_out.csv");
    ASSERT_TRUE(controller.run());

    EXPECT_EQ(readFile("ctrl_empty_out.csv"), "Word,Count,Frequency (%)\n");

    std::remove("ctrl_empty_in.txt");
    std::remove("ctrl_empty_out.csv");
}

TEST(WordCountControllerTest, MissingInputFileFails) {
    WordCountController controller("nonexistent_input.txt", "never_created.csv");
    EXPECT_FALSE(controller.run());
}

TEST(LineParserTest, CommaIsDelimiter) {
    LineParser parser;
    EXPECT_EQ(parser.parse("a,b,c"), (Words{"a", "b", "c"}));
}

TEST(LineParserTest, DotIsDelimiter) {
    LineParser parser;
    EXPECT_EQ(parser.parse("one.two.three"), (Words{"one", "two", "three"}));
}

TEST(LineParserTest, SemicolonAndColonAreDelimiters) {
    LineParser parser;
    EXPECT_EQ(parser.parse("a;b:c"), (Words{"a", "b", "c"}));
}

TEST(LineParserTest, QuestionAndExclamationAreDelimiters) {
    LineParser parser;
    EXPECT_EQ(parser.parse("what?yes!no"), (Words{"what", "yes", "no"}));
}

TEST(LineParserTest, HyphenAndApostropheAreDelimiters) {
    LineParser parser;
    EXPECT_EQ(parser.parse("it's well-known"),
              (Words{"it", "s", "well", "known"}));
}

TEST(LineParserTest, QuotesBracketsAndSlashesAreDelimiters) {
    LineParser parser;
    EXPECT_EQ(parser.parse("\"a\" (b) [c] {d} e/f"),
              (Words{"a", "b", "c", "d", "e", "f"}));
}

TEST(LineParserTest, TabIsDelimiter) {
    LineParser parser;
    EXPECT_EQ(parser.parse("a\tb\tc"), (Words{"a", "b", "c"}));
}

TEST(LineParserTest, ManyDelimitersInRowDoNotCreateEmptyWords) {
    LineParser parser;
    EXPECT_EQ(parser.parse("a,,, ...b"), (Words{"a", "b"}));
}

TEST(LineParserTest, LeadingAndTrailingDelimitersAreIgnored) {
    LineParser parser;
    EXPECT_EQ(parser.parse("  ,hello world.  "), (Words{"hello", "world"}));
}

TEST(LineParserTest, DigitsAreWordCharacters) {
    LineParser parser;
    EXPECT_EQ(parser.parse("abc123 42 7up"), (Words{"abc123", "42", "7up"}));
}

TEST(LineParserTest, UnderscoreIsDelimiter) {
    LineParser parser;
    EXPECT_EQ(parser.parse("snake_case"), (Words{"snake", "case"}));
}

TEST(LineParserTest, LastWordWithoutDelimiterIsKept) {
    LineParser parser;
    EXPECT_EQ(parser.parse("hello world"), (Words{"hello", "world"}));
}