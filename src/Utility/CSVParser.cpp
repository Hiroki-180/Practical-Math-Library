#include <PML/Utility/CSVParser.h>
#include <fstream>

#ifdef _WIN32
#include <locale>
#endif

namespace pml {

    namespace {

        template<typename IterT>
        class ParserBaseImpl : public CSVParser::ParserBase
        {
        protected:
            typedef IterT IterTAlias;
            IterT mStart;
            IterT mEnd;
            std::size_t mLine;
            bool mIsOpen;

        public:

            ParserBaseImpl() :mLine(0), mIsOpen(false)
            {}

            ParserBaseImpl(IterT inStart, IterT inEnd) : mStart(inStart), mEnd(inEnd), mLine(0), mIsOpen(false)
            {}

            virtual ~ParserBaseImpl() override = default;

            virtual bool readNextOneRecord(std::vector<std::string>& outBuffer) override
            {
                outBuffer.clear();

                if (isEnd()  || !isOpen()) {
                    return false;
                }

                bool lIsQuoted   = true;  // Is field enclosed by double quotations.
                bool lIsInQuotes = false; // Is the analyzed character in double quotations.
                bool lIsRight    = false; // Is the analyzed character in right side of the double quotations.

                IterT lit(mStart); // analyzed character
                long lDistanceFromStart = 0;

                std::string lQuotedString  ("");
                std::string lUnQuotedString("");

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

                                return false;
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
                            lIsRight    = true;
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

                                return false;
                            }
                        }
                        else if (lc == ',')
                        {
                            // end of the field
                            outBuffer.push_back(lIsRight ? lQuotedString : lUnQuotedString);
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
                            outBuffer.push_back(lIsRight ? lQuotedString : lUnQuotedString);

                            ++lit;
                            if ((lit != mEnd) && (lc == '\r') && (*lit == '\n')) {
                                ++lit;
                            }

                            mStart = lit;
                            ++mLine;

                            return true;
                        }
                        else if (lIsRight)
                        {
                            // erroe; e.g. \"abc\"d
                            std::advance(lit, -lDistanceFromStart);
                            mStart = lit;
                            SkipToNextLine();

                            return false;
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
                outBuffer.push_back(lIsRight ? lQuotedString : lUnQuotedString);
                ++mLine;

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

            virtual std::size_t getLine() const override
            {
                return mLine;
            }

        private:

            void SkipToNextLine()
            {
                IterT lit(mStart);

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
                ++mLine;
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
                mStart = FileParser::IterTAlias(mFile);

                FileParser::IterTAlias lEOF;
                mEnd = lEOF;
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
                :ParserBaseImpl<std::string::const_iterator>(inString.cbegin(), inString.cend())
            {
                mIsOpen = true;
            }
        };

        class StringParserCopy final : public ParserBaseImpl<std::string::const_iterator>
        {
        public:
            StringParserCopy(const std::string& inString)
                :mString(inString)
            {
                mStart  = mString.cbegin();
                mEnd    = mString.cend();
                mIsOpen = true;
            }

        private:
            std::string mString;
        };

    } // unnamed

    CSVParser::CSVParser(InputType inType, const std::string& inString)
    {
        switch (inType)
        {
        case InputType::FILE:
            mParser.reset(new FileParser(inString));
            break;
        case InputType::STRING_REF:
            mParser.reset(new StringParserRef(inString));
            break;
        case InputType::STRING_COPY:
            mParser.reset(new StringParserCopy(inString));
            break;
        default:
            throw std::runtime_error("Undefined type is specified");
        }
    }

    bool CSVParser::readNextOneRecord(std::vector<std::string>& outBuffer)
    {
        const auto lIsRead = mParser->readNextOneRecord(outBuffer);
        outBuffer.shrink_to_fit();

        return lIsRead;
    }

    bool CSVParser::isOpen() const
    {
        return mParser->isOpen();
    }

    bool CSVParser::isEnd() const
    {
        return  mParser->isEnd();
    }

    std::size_t CSVParser::getLine() const
    {
        return mParser->getLine();
    }

} // pml