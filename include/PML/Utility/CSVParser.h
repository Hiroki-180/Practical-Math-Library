#ifndef UTILITY_CSV_PARSER
#define UTILITY_CSV_PARSER

#include <vector>
#include <map>
#include <string>
#include <memory>

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* CSV format parser.
*/

namespace pml{

    /**
    * @class
    * CSVParser
    *
    * @brief
    * This class read files or string of the CSV format.
    * Here the CSV format is defined as follows;
    *  1. Mixing of newline format of CR, LF, and CRLF is permitted.
    *     That is, each record must be separated by a newline character \n, \r, or \r\n.
    *     \r\n is interpreted as a single newline character of CRLF, not twice newline characters of CR plus LF.
    *  2. Newline character in the end of data is indifferent.
    *  3. Each field must be separated by a single comma(,).
    *  3. The number of fields in each record is not necessarily common.
    *  4. Enclosing a field by two double quotations(") is indifferent.
    *     If the field is not enclosed by double quotations, it is not permitted that the filed contains double quotations " itself.
    *     If the field is enclosed by double quotations, it is permitted by escaping with one further double quotation as "".
    *  5. Spaces and tubs outside double quotations are ignored when the field is enclosed by them.
    */
    class CSVParser final
    {
    public:

        /**
        * Pure virtual base class for implementation of the class CSVParser by composit pattern.
        */
        class ParserBase
        {
        public:
            virtual ~ParserBase() = default;

            virtual bool readNextOneRecord(std::vector<std::string>& outBuffer) = 0;

            virtual bool isEnd() const = 0;

            virtual bool isOpen() const = 0;

            virtual std::size_t getLine() const = 0;
        };

        /**
        * Target data type of the CSV parser.
        */
        enum class InputType
        {
            FILE,
            STRING_REF,
            STRING_COPY,
        };

        /**
        * Unique constructor of this class.
        *
        * @param[in] inType
        * Target CSV data type. Following three types are possible;
        * CSVParser::InputType::FILE, STRING_REF, or STRING_COPY.
        *
        * @param[in] inString
        * If the argument inType is CSVParser::InputType::FILE, inString is interpreted as the path to the target CSV file.
        * If inType is CSVParser::InputType::STRING_REF or STRING_COPY, inString is interpreted as CSV data itself.
        */
        CSVParser(InputType inType, const std::string& inString);

        CSVParser(const CSVParser&)             = delete;
        CSVParser(CSVParser&&)                  = delete;
        CSVParser& operator =(const CSVParser&) = delete;
        CSVParser& operator =(CSVParser&&)      = delete;

        /**
        * Destructor.
        * If the CSV data type is CSVParser::InputType::FILE, the target file is closed here.
        */
        ~CSVParser() = default;

        /**
        * Read the next one record of CSV data.
        *
        * @param[out] outBuffer
        * All Fields of the target one record are pushed back.
        *
        * @return
        * False if and only if data has finished, otherwise true.
        */
        bool readNextOneRecord(std::vector<std::string>& outBuffer);

        /**
        * Read CSV file.
        *
        * @param[in] inFilePath
        * The path to the target CSV file.
        *
        * @return
        * All fields of the target data.
        * outBuffer[i][j] contains j-th field in i-th record.
        */
        static std::vector<std::vector<std::string>> readAllRecords(const std::string& inFilePath);

        /**
        * Read table in CSV format.
        * Suppose the file content as "A,B,C\nD,E,F\G,H,I".
        * Then if inIsColumnKey is true, the result is
        *   outMap[A] = {B,C},
        *   outMap[D] = {E,F},
        *   outMap[G] = {H,I}.
        * On the other hand, if inIsColumnKey is false, the result is
        *   outMap[A] = {D,G},
        *   outMap[B] = {E,H},
        *   outMap[C] = {F,I}.
        *
        * @param[in] inFilePath
        * The path to the target CSV file.
        *
        * @param[in] inIsColumnKey
        * Whether the first column is keys or not.
        * If this is false, the first row is interpreted as keys.
        *
        * @return
        * Resulted table as map.
        */
        static std::map<std::string, std::vector<std::string>> readTable(
            const std::string& inFilePath,
            bool inIsColumnKey);

        /**
        * Target file or string is correctly set or not.
        *
        * @return
        * True if target is correctly set.
        */
        bool isOpen() const;

        /**
        * Whether all records are read or not.
        *
        * @return
        * True if the next charater is EOF or the tarmination character.
        */
        bool isEnd() const;

        /**
        * The number of already read records.
        *
        * @return
        * The number of already read records.
        */
        std::size_t getLine() const;

    private:

        /**
        * Composit of implementation.
        */
        std::unique_ptr<ParserBase> mParser;
    };

} // pml

#endif
