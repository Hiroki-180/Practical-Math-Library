#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/pmath.h>
#include <chrono>
#include <cmath>
#include <functional>

namespace {

    struct SumAndPerformance final
    {
        SumAndPerformance(double inSum, long long inElapsedTime)
            : mSum(inSum), mElapsedTime(inElapsedTime)
        {}

        double mSum;
        long long mElapsedTime;
    };

    SumAndPerformance test(const std::function<double(double)>& inF)
    {
        const auto lTestNum
#ifdef NDEBUG
            = 10000000;
#else
            = 100000;
#endif

        auto lSum = 0.0;

        const auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            const auto lX = static_cast<double>(i*0.00001);
            lSum += inF(lX);
        }
        const auto lEnd = std::chrono::system_clock::now();
        const auto lElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        return SumAndPerformance(lSum, lElapsedTime);
    };
}

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

    const auto& lSTLF = std::get<1>(GetParam());
    const auto lSTLResult = test(lSTLF);

    const auto& lPMLF = std::get<2>(GetParam());
    const auto lPMLResult = test(lPMLF);

    EXPECT_NEAR(lSTLResult.mSum, lPMLResult.mSum, std::fabs(lPMLResult.mSum)*1.0E-13);

    std::cout
#ifdef NDEBUG
        << "---Release Mode---\n"
#else
        << "---Debud Mode---\n"
#endif
        << std::setprecision(3)
        << "STL:" << lSTLResult.mElapsedTime << "[msec],\n"
        << "PML:" << lPMLResult.mElapsedTime << "[msec],\n"
        << "PML is " << (lSTLResult.mElapsedTime / (double)(lPMLResult.mElapsedTime))
        << " times faster than this compiler's STL implimentation.\n";

#ifdef NDEBUG
    EXPECT_LE(lPMLResult.mElapsedTime - lSTLResult.mElapsedTime, 0.1*lSTLResult.mElapsedTime);
#endif
}

INSTANTIATE_TEST_CASE_P(
    name, pmath,
    ::testing::Values(
        std::make_tuple(
            "sin",
            std::bind((double(*)(double))&std::sin, std::placeholders::_1),
            std::bind(&pml::sin, std::placeholders::_1)),
        std::make_tuple(
            "cos",
            std::bind((double(*)(double))&std::cos, std::placeholders::_1),
            std::bind(&pml::cos, std::placeholders::_1)),
        std::make_tuple(
            "exp",
            std::bind((double(*)(double))&std::exp, std::placeholders::_1),
            std::bind(&pml::exp, std::placeholders::_1))));
