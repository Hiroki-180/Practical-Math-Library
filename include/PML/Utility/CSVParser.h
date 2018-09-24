#ifndef UTILITY_CSV_PARSER
#define UTILITY_CSV_PARSER

#include <vector>
#include <string>
#include <memory>

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* CSV file or string reader.
*/

namespace pml{

    class CSVParser
    {
    public:

        class ParserBase
        {
        public:
            virtual ~ParserBase() = default;

            virtual bool readOneLine(std::vector<std::string>& outBuffer) = 0;

            virtual bool isEnd() = 0;

            virtual bool isOpen() = 0;

            virtual std::size_t getLine() = 0;
        };

        enum class InputType
        {
            FILE,
            STRING_REF,
            STRING_COPY,
        };

        CSVParser(InputType inType, const std::string& inString);
        CSVParser(const CSVParser&)             = delete;
        CSVParser(CSVParser&&)                  = delete;
        CSVParser& operator =(const CSVParser&) = delete;
        CSVParser& operator =(CSVParser&&)      = delete;

        ~CSVParser() = default;

        bool readOneLine(std::vector<std::string>& outBuffer);

        bool isOpen();

        bool isEnd();

        std::size_t getLine();

    private:
        std::unique_ptr<ParserBase> mParser;
    };

} // pml

#endif
