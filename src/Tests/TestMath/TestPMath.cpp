#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/pmath.h>

#include <chrono>
#include <cmath>
#include <functional>

TEST(TestPMath, exp_value)
{
    for (auto i = 0; i < 200; ++i)
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
    for (auto i = 0; i < lTestNum; ++i)
    {
        const auto lX = -5.0 + lStride * i;
        lSum += std::exp(lX);
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    auto lSumPML = 0.0;
    const auto lStartPML = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        const auto lX = -5.0 + lStride * i;
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
        << "PML:" << lElapsedPML << "[msec],\n"
        << std::setprecision(3)
        << "PML is " << (lElapsed / (double)(lElapsedPML)) << " faster than this compiler's STL implimentation.\n";

#ifdef NDEBUG
    EXPECT_LE(lElapsedPML, lElapsed);
#endif
}

TEST(TestPMath, expv_value)
{
    std::vector<double> lExponent;

    for (auto i = 0; i < 200; ++i)
    {
        lExponent.push_back(-5.0 + 0.05*i);
    }

    std::vector<double> lResult(lExponent.size());
    pml::expv(lExponent.data(), lResult.data(), lExponent.size());

    for (auto i = 0; i < 200; ++i)
    {
        EXPECT_DOUBLE_EQ(std::exp(lExponent[i]), lResult[i]);
    }
}


class pmath_trigonometric : public ::testing::TestWithParam<
    std::pair<std::function<double(double)>, std::function<double(double)>>> {};

TEST_P(pmath_trigonometric, value)
{
    const auto& lSTLF = GetParam().first;
    const auto& lPMLF = GetParam().second;

    for (auto i = -1000000; i < 1000000; ++i)
    {
        const auto lX = static_cast<double>(i*0.0001);
        EXPECT_DOUBLE_EQ(lSTLF(lX), lPMLF(lX)) << lX;
    }
}

TEST_P(pmath_trigonometric, performance)
{
    const auto lTestNum
#ifdef NDEBUG
        = 10000000;
#else
        = 100000;
#endif

    auto lSum = 0.0;
    const auto& lSTLF = GetParam().first;
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        const auto lX = static_cast<double>(i*0.00001);
        lSum += lSTLF(lX);
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    auto lSumPML = 0.0;
    const auto& lPMLF = GetParam().second;
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
    name, pmath_trigonometric,
    ::testing::Values(
        std::make_pair(
            std::bind((double(*)(double))&std::sin, std::placeholders::_1),
            std::bind((double(*)(double))&pml::sin, std::placeholders::_1)),
        std::make_pair(
            std::bind((double(*)(double))&std::cos, std::placeholders::_1),
            std::bind((double(*)(double))&pml::cos, std::placeholders::_1))));