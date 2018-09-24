#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Utility/CSVParser.h>

class CSVParserString : public ::testing::TestWithParam<std::pair<std::string,std::vector<std::string>>> {};

TEST_P(CSVParserString, refAndCopy)
{
    std::vector<pml::CSVParser::InputType> lTypes
        = { pml::CSVParser::InputType::STRING_REF ,pml::CSVParser::InputType::STRING_COPY };

    for (const auto& type_i : lTypes)
    {
        pml::CSVParser lParser(type_i, GetParam().first);
        std::vector<std::string> lBuffer;

        EXPECT_TRUE(lParser.isOpen());
        EXPECT_FALSE(lParser.isEnd());
        EXPECT_EQ(0U, lParser.getLine());

        auto lLines = 0U;
        auto lIdx = 0U;
        while (lParser.readOneLine(lBuffer))
        {
            for (auto i = 0U; i < lBuffer.size(); ++i) {
                EXPECT_EQ(GetParam().second[lIdx], lBuffer[i]);
                ++lIdx;
            }

            ++lLines;
        }

        EXPECT_TRUE(lParser.isEnd());
        EXPECT_TRUE(lParser.isOpen());
        EXPECT_EQ(lLines, lParser.getLine());
    }
}

INSTANTIATE_TEST_CASE_P(
    name, CSVParserString,
    ::testing::Values(
        std::make_pair(std::string("abc,def,ghi"), std::vector<std::string>{"abc", "def", "ghi"}),
        std::make_pair(std::string("\"abc\",\"def\",\"ghi\""), std::vector<std::string>{ "abc", "def", "ghi" }),
        std::make_pair(std::string("abc,\"def\",ghi"), std::vector<std::string>{ "abc", "def", "ghi" }),
        std::make_pair(std::string("abc, \"def\" ,ghi"), std::vector<std::string>{ "abc", "def", "ghi" }),
        std::make_pair(std::string("abc,\" de f \",ghi"), std::vector<std::string>{ "abc", " de f ", "ghi" }),
        std::make_pair(std::string("abc,\" de f \",ghi\n"), std::vector<std::string>{ "abc", " de f ", "ghi" }),
        std::make_pair(std::string("abc,\" de f \",ghi\r"), std::vector<std::string>{ "abc", " de f ", "ghi" }),
        std::make_pair(std::string("abc,\" de f \",ghi\r\n"), std::vector<std::string>{ "abc", " de f ", "ghi" }),
        std::make_pair(std::string("abc,def,ghi\njkl,mn,op"), std::vector<std::string>{ "abc", "def", "ghi","jkl","mn","op" }),
        std::make_pair(std::string("abc,def,ghi\rjkl,mn,op"), std::vector<std::string>{ "abc", "def", "ghi", "jkl", "mn", "op" }),
        std::make_pair(std::string("abc,def,ghi\r\njkl,mn,op"), std::vector<std::string>{ "abc", "def", "ghi", "jkl", "mn", "op" }),
        std::make_pair(std::string("abc,def,ghi\njkl,mn,op\n"), std::vector<std::string>{ "abc", "def", "ghi", "jkl", "mn", "op" }),
        std::make_pair(std::string("abc,def,ghi\rjkl,mn,op\r"), std::vector<std::string>{ "abc", "def", "ghi", "jkl", "mn", "op" }),
        std::make_pair(std::string("abc,def,ghi\r\njkl,mn,op\r\n"), std::vector<std::string>{ "abc", "def", "ghi", "jkl", "mn", "op" }),
        std::make_pair(std::string("ab,cd,ef\ngh,ij,kl\nmn,op"), std::vector<std::string>{ "ab", "cd", "ef", "gh", "ij", "kl","mn","op" })));