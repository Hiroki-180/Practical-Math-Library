#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/pmath.h>

#include <chrono>

TEST(TestPMath, exp_value)
{
    for (auto i = 0;i < 200;++i)
    {
        const auto lX = -5.0 + 0.05*i;
        EXPECT_DOUBLE_EQ(std::exp(lX), pml::exp(lX));
    }
}

TEST(TestPMath, exp_performance)
{
    const auto lTestNum
#ifdef NDEBUG
        = 3000000;
#else
        = 100000;
#endif

    const auto lStride = 10.0 / lTestNum;

    auto lSum = 0.0;
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0;i < lTestNum;++i)
    {
        const auto lX = -5.0 + lStride *i;
        lSum += std::exp(lX);
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    auto lSumPML = 0.0;
    const auto lStartPML = std::chrono::system_clock::now();
    for (auto i = 0;i < lTestNum;++i)
    {
        const auto lX = -5.0 + lStride *i;
        lSumPML += pml::exp(lX);
    }
    const auto lEndPML = std::chrono::system_clock::now();
    const auto lElapsedPML = std::chrono::duration_cast<std::chrono::milliseconds>(lEndPML - lStartPML).count();

    EXPECT_DOUBLE_EQ(lSum, lSumPML);

    std::cout
#ifdef NDEBUG
        << "---Release Mode---\n"
#else
        << "---Debud Mode---\n"
#endif
        << "STL:" << lElapsed << "[msec],\n"
        << "PML:" << lElapsedPML << "[msec],\n";

#ifdef NDEBUG
    EXPECT_LE(lElapsedPML, lElapsed);
#endif
}