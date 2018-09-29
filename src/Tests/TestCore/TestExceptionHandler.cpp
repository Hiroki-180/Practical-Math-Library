#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/exception_handler.h>
#include <PML/Utility/CSVParser.h>
#include <fstream>

namespace{

    [[noreturn]] void h()
    {
        PML_THROW_WITH_NESTED(std::logic_error, "Error 1.");
    }

    [[noreturn]] void g()
    {
        try {
            h();
        }
        catch (...) {
            PML_THROW_WITH_NESTED(std::runtime_error, "Error 2.");
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
    EXPECT_EQ(3U, lResult.size());
}

