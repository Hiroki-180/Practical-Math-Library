#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/pmath.h>

#include <chrono>
#include <cmath>

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

TEST(TestPMath, sin_value)
{
    for (auto i = -20000; i < 20000; ++i)
    {
        const auto lX = static_cast<double>(i);
        EXPECT_DOUBLE_EQ(std::sin(lX), pml::sin(lX));
    }
}

TEST(TestPMath, sin_performance)
{
    const auto lTestNum
#ifdef NDEBUG
        = 2000000;
#else
        = 100000;
#endif

    auto lSum = 0.0;
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        lSum += std::sin(static_cast<double>(i));
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    auto lSumPML = 0.0;
    const auto lStartPML = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        lSumPML += pml::sin(static_cast<double>(i));
    }
    const auto lEndPML = std::chrono::system_clock::now();
    const auto lElapsedPML = std::chrono::duration_cast<std::chrono::milliseconds>(lEndPML - lStartPML).count();

    EXPECT_NEAR(lSum, lSumPML, 1.0E-12);

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

TEST(TestPMath, cos_value)
{
    for (auto i = -20000; i < 20000; ++i)
    {
        const auto lX = static_cast<double>(i);
        EXPECT_DOUBLE_EQ(std::cos(lX), pml::cos(lX));
    }
}

TEST(TestPMath, cos_performance)
{
    const auto lTestNum
#ifdef NDEBUG
        = 2000000;
#else
        = 100000;
#endif

    auto lSum = 0.0;
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        lSum += std::cos(static_cast<double>(i));
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    auto lSumPML = 0.0;
    const auto lStartPML = std::chrono::system_clock::now();
    for (auto i = 0; i < lTestNum; ++i)
    {
        lSumPML += pml::cos(static_cast<double>(i));
    }
    const auto lEndPML = std::chrono::system_clock::now();
    const auto lElapsedPML = std::chrono::duration_cast<std::chrono::milliseconds>(lEndPML - lStartPML).count();

    EXPECT_NEAR(lSum, lSumPML, 1.0E-12);

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
