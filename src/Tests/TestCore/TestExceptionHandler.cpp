#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/exception_handler.h>
#include <PML/Utility/CSVParser.h>
#include <fstream>
#include <sstream>
#include <functional>
#include <cstdio>

namespace{

    [[noreturn]] void h()
    {
        QHMACRO_THROW_WITH_NESTED(std::logic_error, "Error in h().");
    }

    void g()
    {
        QHMACRO_HOOK(h(), std::runtime_error, "Error in g().");
    };

    void f()
    {
        QHMACRO_HOOK(g(), std::runtime_error, "Error in f().");
    }
} // unnamed


class exception_handler : public ::testing::TestWithParam<std::pair<std::function<void()>, std::size_t>> {};

TEST_P(exception_handler, nest_output)
{
    const std::string lFileName("Test_exception_handler_nest_output.csv");

    std::stringstream lSs_1;
    {
        {
            std::ofstream lOfs(lFileName);
            EXPECT_TRUE(lOfs.is_open());
            QHMACRO_CATCH_BEGIN
            (GetParam().first)();
            QHMACRO_CATCH_END(lOfs)

            auto lResult = pml::CSVParser::readAllRecords(lFileName);
            EXPECT_EQ(GetParam().second, lResult.size());
        }

        {
            std::ifstream lIfs(lFileName);
            lSs_1 << lIfs.rdbuf();
        }

        remove(lFileName.c_str());
    }

    std::stringstream lSs_2;
    QHMACRO_CATCH_BEGIN
        (GetParam().first)();
    QHMACRO_CATCH_END(lSs_2)
    EXPECT_EQ(lSs_1.str(), lSs_2.str());
}

TEST_P(exception_handler, nest_rethrow)
{
    const std::string lFileName("Test_exception_handler_nest_rethrow.csv");

    std::ofstream lOfs(lFileName);
    EXPECT_TRUE(lOfs.is_open());
    QHMACRO_CATCH_BEGIN
    QHMACRO_HOOK((GetParam().first)(), std::runtime_error, "This is a test.");
    QHMACRO_CATCH_END(lOfs)

    auto lResult = pml::CSVParser::readAllRecords(lFileName);
    EXPECT_EQ(((GetParam().second == 0) ? 0U : (GetParam().second + 1U)), lResult.size());

    remove(lFileName.c_str());
}

INSTANTIATE_TEST_CASE_P(
    exception, exception_handler,
    ::testing::Values(
        std::make_pair([]() { }, 0U),
        std::make_pair([]() { throw std::runtime_error("Runtime error."); }, 1U),
        std::make_pair([]() { throw std::bad_alloc(); }, 1U),
        std::make_pair(std::bind(&h), 1U),
        std::make_pair(std::bind(&g), 2U),
        std::make_pair(std::bind(&f), 3U)));
