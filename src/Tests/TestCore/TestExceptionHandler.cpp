#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/exception_handler.h>
#include <PML/Utility/CSVParser.h>

namespace{

    [[noreturn]] void h()
    {
        throw std::bad_alloc();
    }

    [[noreturn]] void g()
    {
        try {
            h();
        }
        catch (...) {
            PML_THROW_WITH_NESTED(std::logic_error, "Error 2.");
        }
    };

    [[noreturn]] void f()
    {
        try {
            g();
        }
        catch (...) {
            PML_THROW_WITH_NESTED(std::runtime_error, "Error 3.");
        }
    }
}

TEST(exception_handler, nest)
{
    std::ofstream lOfs("Test.csv");
    EXPECT_TRUE(lOfs.is_open());

    PML_CATCH_BEGIN
        f();
    PML_CATCH_END_AND_PRINT(lOfs)

    auto lResult = pml::CSVParser::readAllRecords("Test.csv");
    auto lAns    = pml::CSVParser::readAllRecords("Test_Ans.csv");
    EXPECT_EQ(lAns, lResult);
}

