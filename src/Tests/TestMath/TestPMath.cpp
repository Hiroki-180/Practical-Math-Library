#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/pmath.h>

#include <chrono>
#include <cmath>
#include <functional>

class pmath : public ::testing::TestWithParam<
    std::tuple<std::string, std::function<double(double)>, std::function<double(double)>>> {};

TEST_P(pmath, value)
{
    const auto& lSTLF = std::get<1>(GetParam());
    const auto& lPMLF = std::get<2>(GetParam());

    for (auto i = -10000; i < 10000; ++i)
    {
        const auto lX = static_cast<double>(i*0.0001);
        EXPECT_DOUBLE_EQ(lSTLF(lX), lPMLF(lX)) << std::get<0>(GetParam()) << "(" << lX <<")\n";
    }
}

TEST_P(pmath, performance)
{
    std::cout << "Test of " << std::get<0>(GetParam()) << " function.\n";

    const auto lTestNum
#ifdef NDEBUG
        = 10000000;
#else
        = 100000;
#endif

    auto lSum = 0.0;
    const auto& lSTLF = std::get<1>(GetParam());
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        const auto lX = static_cast<double>(i*0.00001);
        lSum += lSTLF(lX);
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    auto lSumPML = 0.0;
    const auto& lPMLF = std::get<2>(GetParam());
    const auto lStartPML = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        const auto lX = static_cast<double>(i*0.00001);
        lSumPML += lPMLF(lX);
    }
    const auto lEndPML = std::chrono::system_clock::now();
    const auto lElapsedPML = std::chrono::duration_cast<std::chrono::milliseconds>(lEndPML - lStartPML).count();

    EXPECT_NEAR(lSum, lSumPML, std::fabs(lSum)*1.0E-13);

    std::cout
#ifdef NDEBUG
        << "---Release Mode---\n"
#else
        << "---Debud Mode---\n"
#endif
        << "STL:" << lElapsed << "[msec],\n"
        << "PML:" << lElapsedPML << "[msec],\n"
        << std::setprecision(3)
        << "PML is " << (lElapsed / (double)(lElapsedPML)) << " times faster than this compiler's STL implimentation.\n";

#ifdef NDEBUG

    EXPECT_LE(lElapsedPML - lElapsed, 0.1*lElapsed);
#endif
}

INSTANTIATE_TEST_CASE_P(
    name, pmath,
    ::testing::Values(
        std::make_tuple(
            "sin",
            std::bind((double(*)(double))&std::sin, std::placeholders::_1),
            std::bind((double(*)(double))&pml::sin, std::placeholders::_1)),
        std::make_tuple(
            "cos",
            std::bind((double(*)(double))&std::cos, std::placeholders::_1),
            std::bind((double(*)(double))&pml::cos, std::placeholders::_1)),
        std::make_tuple(
            "exp",
            std::bind((double(*)(double))&std::exp, std::placeholders::_1),
            std::bind((double(*)(double))&pml::exp, std::placeholders::_1))));