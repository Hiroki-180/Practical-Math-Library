#ifndef UTILITY_CSV_PARSER
#define UTILITY_CSV_PARSER

/**
* @file
* public header provided by PML.
*
* @brief
* CSV format parser.
*/

#include <PML/Core/exception_handler.h>

#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#ifdef _WIN32
#include <locale>
#endif

namespace pml{

    /**
    * @class
    * CSVParser
    *
    * @brief
    * This class reads files or string of the CSV format.
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
        * @brief
        * Read CSV file.
        *
        * @param[in] inFilePath
        * The path to the target CSV file.
        *
        * @return
        * All fields of the target data.
        * outBuffer[i][j] contains j-th field in i-th record.
        */
        static std::vector<std::vector<std::string>> readAllRecords(const std::string& inFilePath)
        {
            QHMACRO_CATCH_BEGIN

            std::vector<std::vector<std::string>> lOutBuffer;

            CSVParser lParser(CSVParser::InputType::FILE, inFilePath);
            std::vector<std::string> lRecord;

            while (lParser.readNextOneRecord(lRecord))
            {
                lRecord.shrink_to_fit();
                lOutBuffer.push_back(std::move(lRecord));
            }

            return lOutBuffer;

            QHMACRO_CATCH_END_AND_THROW(std::runtime_error, "CSVParser::readAllRecords failed.")
        }

        /**
        * @brief
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
        * Resulted table as unordered_map.
        */
        static std::unordered_map<std::string, std::vector<std::string>> readTable(
            const std::string& inFilePath,
            bool inIsColumnKey)
        {
            return QHMACRO_HOOK(
                readTableImpl<std::unordered_map>(inFilePath, inIsColumnKey),
                std::runtime_error, "CSVParser::readTable failed.");
        }

        /**
        * @brief
        * Target data type of the CSV parser.
        */
        enum class InputType
        {
            FILE,
            STRING_REF,
            STRING_COPY,
        };

        /**
        * @brief
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
        CSVParser(InputType inType, const std::string& inString)
        {
            switch (inType)
            {
            case InputType::FILE:
                mParser = std::make_unique<FileParser>(inString);
                break;
            case InputType::STRING_REF:
                mParser = std::make_unique<StringParserRef>(inString);
                break;
            case InputType::STRING_COPY:
                mParser = std::make_unique<StringParserCopy>(inString);
                break;
            default:
                QHMACRO_THROW_WITH_NESTED(std::logic_error, "Undefined type is specified.");
            }
        }

        CSVParser(const CSVParser&)              = delete;
        CSVParser(CSVParser&&)                   = delete;
        CSVParser & operator =(const CSVParser&) = delete;
        CSVParser & operator =(CSVParser&&)      = delete;

        /**
        * @brief
        * Destructor.
        * If the CSV data type is CSVParser::InputType::FILE, the target file is closed here.
        */
        ~CSVParser() = default;

        /**
        * @brief
        * Read the next one record of CSV data.
        *
        * @param[out] outBuffer
        * All Fields of the target one record are pushed back.
        * outBuffer is cleared at the begining of this function.
        *
        * @return
        * False if and only if data has alreadyfinished, otherwise true.
        */
        bool readNextOneRecord(std::vector<std::string>& outBuffer)
        {
            const auto lIsRead = mParser->readNextOneRecord(outBuffer);
            outBuffer.shrink_to_fit();

            return lIsRead;
        }

        /**
        * @brief
        * Target file or string is correctly set or not.
        *
        * @return
        * True if target is correctly set.
        */
        bool isOpen() const
        {
            return mParser->isOpen();
        }

        /**
        * @brief
        * Whether all records are read or not.
        *
        * @return
        * True if the next charater is EOF or the tarmination character.
        */
        bool isEnd() const
        {
            return mParser->isEnd();
        }

        /**
        * @brief
        * The number of already read records.
        *
        * @return
        * The number of already read records.
        */
        std::size_t getLineNumber() const
        {
            return mParser->getLineNumber();
        }

    private:

        /**
        * @brief
        * Pure virtual base class for implementation of the class CSVParser by composit pattern.
        */
        class ParserBase
        {
        public:
            virtual ~ParserBase() = default;

            virtual bool readNextOneRecord(std::vector<std::string>& outBuffer) = 0;

            virtual bool isEnd() const = 0;

            virtual bool isOpen() const = 0;

            virtual std::size_t getLineNumber() const = 0;
        };

        template<typename Iterator>
        class ParserBaseImpl : public ParserBase
        {
        protected:
            using iterator_type = Iterator;
            Iterator mStart;
            Iterator mEnd;
            std::size_t mLineNumber;
            bool mIsOpen;

        public:

            ParserBaseImpl() : mLineNumber(0), mIsOpen(false)
            {}

            ParserBaseImpl(Iterator inStart, Iterator inEnd) : mStart(inStart), mEnd(inEnd), mLineNumber(0), mIsOpen(false)
            {}

            virtual ~ParserBaseImpl() override = default;

            virtual bool readNextOneRecord(std::vector<std::string>& outBuffer) override
            {
                outBuffer.clear();

                if (!isOpen()) {
                    QHMACRO_THROW_WITH_NESTED(std::runtime_error, "Taregt is not opened.");
                }

                if (isEnd()) {
                    return false;
                }

                bool lIsQuoted   = true;  // Is field enclosed by double quotations.
                bool lIsInQuotes = false; // Is the analyzed character in double quotations.
                bool lIsRight    = false; // Is the analyzed character in right side of the double quotations.

                Iterator lit(mStart); // analyzed character
                long lDistanceFromStart = 0;

                std::string lQuotedString;
                std::string lUnQuotedString;

                while (lit != mEnd)
                {
                    while (!lIsInQuotes && (lit != mEnd) && (*lit == ' ' || *lit == '\t'))
                    {
                        lUnQuotedString.push_back(*lit);
                        ++lit;
                        ++lDistanceFromStart;
                    }

                    if (lit == mEnd) {
                        break;
                    }

                    if (lIsInQuotes)
                    {
                        for (; *lit != '\"';)
                        {
                            lQuotedString.push_back(*lit);
                            ++lit;
                            ++lDistanceFromStart;

                            if (lit == mEnd)
                            {
                                std::advance(lit, -lDistanceFromStart);
                                mStart = lit;
                                SkipToNextLine();

                                QHMACRO_THROW_WITH_NESTED(
                                    std::runtime_error, "Field is not closed by right side double quotation.");
                            }
                        }

                        ++lit;
                        ++lDistanceFromStart;

                        if ((lit != mEnd) && (*lit == '\"'))
                        {
                            // encountered 2 continuous double quotes in a string and resolve them to 1 double quote
                            lQuotedString.push_back('\"');
                            ++lit;
                            ++lDistanceFromStart;
                        }
                        else
                        {
                            // end quoted char
                            lIsInQuotes = false;
                            lIsRight = true;
                        }
                    }
                    else
                    {
                        char lc = *lit;

                        if (lc == '\"')
                        {
                            if (lQuotedString.empty() && lIsQuoted)
                            {
                                // begin quoted string
                                lIsInQuotes = true;
                                ++lit;
                                ++lDistanceFromStart;
                            }
                            else
                            {
                                // error; e.g. \"abc\"(spaces)\"
                                std::advance(lit, -lDistanceFromStart);
                                mStart = lit;
                                SkipToNextLine();

                                QHMACRO_THROW_WITH_NESTED(
                                    std::runtime_error, "Double quotation as an element of fields must be escaped by itself.");
                            }
                        }
                        else if (lc == ',')
                        {
                            // end of the field
                            outBuffer.push_back(std::move(lIsRight ? lQuotedString : lUnQuotedString));
                            lQuotedString  .clear();
                            lUnQuotedString.clear();

                            lIsQuoted = true;
                            lIsRight = false;
                            ++lit;
                            mStart = lit;
                            lDistanceFromStart = 0;
                        }
                        else if ((lc == '\r') || (lc == '\n'))
                        {
                            // end of the record
                            outBuffer.push_back(std::move(lIsRight ? lQuotedString : lUnQuotedString));

                            ++lit;
                            if ((lit != mEnd) && (lc == '\r') && (*lit == '\n')) {
                                ++lit;
                            }

                            mStart = lit;
                            ++mLineNumber;

                            return true;
                        }
                        else if (lIsRight)
                        {
                            // erroe; e.g. \"abc\"d
                            std::advance(lit, -lDistanceFromStart);
                            mStart = lit;
                            SkipToNextLine();

                            QHMACRO_THROW_WITH_NESTED(
                                std::runtime_error, "Character exist in the field " + lQuotedString + " outside of double quotations.");
                        }
                        else
                        {
                            lIsQuoted = false;
                            lUnQuotedString.push_back(lc);

                            ++lit;
                            ++lDistanceFromStart;
                        }
                    }
                }

                // finished without a newline character
                outBuffer.push_back(std::move(lIsRight ? lQuotedString : lUnQuotedString));
                ++mLineNumber;

                mStart = mEnd;
                return true;
            }

            virtual bool isEnd() const override
            {
                return (mStart == mEnd);
            }

            virtual bool isOpen() const override
            {
                return mIsOpen;
            }

            virtual std::size_t getLineNumber() const override
            {
                return mLineNumber;
            }

        private:

            void SkipToNextLine()
            {
                auto lit(mStart);

                while ((*lit != '\r') && (*lit != '\n'))
                {
                    if (lit == mEnd)
                    {
                        mStart = mEnd;
                        return;
                    }
                }

                char lc = *lit;
                ++lit;

                if ((lit != mEnd) && (lc == '\r') && (*lit == '\n')) {
                    ++lit;
                }

                mStart = lit;
                ++mLineNumber;
            }
        };

        class FileParser final : public ParserBaseImpl<std::istreambuf_iterator<char>>
        {
        public:
            explicit FileParser(const std::string& inFileName)
            {
#ifdef _WIN32
                std::locale lLC_DEF;
                std::locale::global(std::locale(""));
                mFile.open(inFileName.c_str(), std::ios::in);
                std::locale::global(std::locale(lLC_DEF));
#else
                mFile.open(inFileName.c_str(), std::ios::in);
#endif

                mIsOpen = !(mFile.fail());

                mStart = FileParser::iterator_type(mFile);
                mEnd   = FileParser::iterator_type();
            }

            virtual ~FileParser() override
            {
                mFile.close();
            }

        private:
            std::ifstream mFile;
        };

        class StringParserRef final : public ParserBaseImpl<std::string::const_iterator>
        {
        public:
            StringParserRef(const std::string& inString)
                : ParserBaseImpl<std::string::const_iterator>(inString.cbegin(), inString.cend())
            {
                mIsOpen = true;
            }
        };

        class StringParserCopy final : public ParserBaseImpl<std::string::const_iterator>
        {
        public:
            StringParserCopy(const std::string& inString)
                : mString(inString)
            {
                mStart = mString.cbegin();
                mEnd   = mString.cend();
                mIsOpen = true;
            }

        private:
            std::string mString;
        };

        template<template <typename...> class Map>
        static auto readTableImpl(
            const std::string& inFilePath,
            bool inIsColumnKey)
        {
            Map<std::string, std::vector<std::string>> lTable;

            CSVParser lParser(CSVParser::InputType::FILE, inFilePath);
            std::vector<std::string> lRecord;

            if (inIsColumnKey)
            {
                while (lParser.readNextOneRecord(lRecord))
                {
                    if (lRecord.empty()) {
                        continue;
                    }

                    auto lKey = std::move(lRecord[0]);
                    lRecord.erase(lRecord.cbegin());

                    lTable.emplace(std::move(lKey), std::move(lRecord));
                }
            }
            else
            {
                std::vector<std::string> lKeys;
                lParser.readNextOneRecord(lKeys);

                std::vector<std::vector<std::string>> lVals(lKeys.size());

                for (;;)
                {
                    if (!lParser.readNextOneRecord(lRecord)) {
                        break;
                    }

                    if (lRecord.size() != lKeys.size()) {
                        QHMACRO_THROW_WITH_NESTED(std::runtime_error, "Record size is unmatched with the key size.");
                    }

                    for (std::size_t i = 0; i < lRecord.size(); ++i) {
                        lVals[i].push_back(std::move(lRecord[i]));
                    }
                }

                for (std::size_t i = 0; i < lKeys.size(); ++i)
                {
                    lVals[i].shrink_to_fit();
                    lTable.emplace(std::move(lKeys[i]), std::move(lVals[i]));
                }
            }

            return lTable;
        }

        /**
        * @brief
        * Composit member of implementation.
        */
        std::unique_ptr<ParserBase> mParser;
    };

} // pml

#endif
