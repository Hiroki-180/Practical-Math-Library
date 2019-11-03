#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Core/CPUDispatcher.h>
#include <numeric>
#include <chrono>
#include <cmath>

namespace {

    constexpr auto TEST_NUM
#ifdef NDEBUG
        = 1000000;
#else
        = 5000;
#endif

    constexpr std::size_t TEST_ARRAY_SIZE = 1000;

    template<typename T>
    void outputResult(
        const T& inAns,
        const T& inOptSIMDVResult,
        const T& inOptSIMDAResult,
        const long long inElapsedTime,
        const long long inElapsedTimeOptSIMDV,
        const long long inElapsedTimeOptSIMDA)
    {
        EXPECT_EQ(inAns, inOptSIMDVResult);
        EXPECT_EQ(inAns, inOptSIMDAResult);

        std::cout
#ifdef NDEBUG
            << "---Release Mode---\n"
#else
            << "---Debud Mode---\n"
#endif
            << TEST_NUM << "-times calculation,\n"
            << TEST_ARRAY_SIZE << "-elements array,\n"
            << "Naive               :" << inElapsedTime << "[msec],\n"
            << "Opt.SIMD std::vector:" << inElapsedTimeOptSIMDV << "[msec],\n"
            << "Opt.SIMD std::array :" << inElapsedTimeOptSIMDA << "[msec].\n";

#ifdef NDEBUG
        EXPECT_LT(inElapsedTimeOptSIMDV,  inElapsedTime);
        EXPECT_LT(inElapsedTimeOptSIMDA,  inElapsedTime);
#endif
    }
}

TEST(TestNumericSIMD, accumulate)
{
	auto lInitializer = [](auto& inArray)->void
	{
		for (std::size_t i = 0; i < inArray.size(); ++i){
			inArray[i] = static_cast<double>(i);
		}
	};

    // STL.
    auto lSum = 0.0;
    long long lElapsed = 0;

    // automatically selected optimal SIMD with std::vector.
    auto lSumOptSIMDV = 0.0;
    long long lOptSIMDVElapsed = 0;

    // automatically selected optimal SIMD with array.
    auto lSumOptSIMDA = 0.0;
    long long lOptSIMDAElapsed = 0;

    {
        std::vector<double> lVector(TEST_ARRAY_SIZE);
        lInitializer(lVector);

        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < TEST_NUM; ++i){
            lSum = std::accumulate(lVector.cbegin(), lVector.cend(), 0.0);
        }
        auto lEnd = std::chrono::system_clock::now();
        lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < TEST_NUM; ++i){
            lSumOptSIMDV = pml::accumulate_SIMD(lVector, 0.0);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        std::array<double, TEST_ARRAY_SIZE> lArr;
        lInitializer(lArr);
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < TEST_NUM; ++i){
            lSumOptSIMDA = pml::accumulate_SIMD(lArr, 0.0);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

	outputResult(
        lSum, lSumOptSIMDV, lSumOptSIMDA,
        lElapsed, lOptSIMDVElapsed, lOptSIMDAElapsed);
}

TEST(TestNumericSIMD, inner_product)
{
    auto lInitializer = [](auto& inArray1, auto& inArray2)->void
    {
        for (std::size_t i = 0; i < inArray1.size(); ++i)
        {
            inArray1[i] = 1.0;
            inArray2[i] = static_cast<double>(i);
        }
    };

    // STL.
    auto lSum = 0.0;
    long long lElapsed = 0;

    // automatically selected optimal SIMD with std::vector.
    auto lSumOptSIMDV = 0.0;
    long long lOptSIMDVElapsed = 0;

    // automatically selected optimal SIMD with array.
    auto lSumOptSIMDA = 0.0;
    long long lOptSIMDAElapsed = 0;

    {
        std::vector<double> lVector1(TEST_ARRAY_SIZE);
        std::vector<double> lVector2(TEST_ARRAY_SIZE);
        lInitializer(lVector1, lVector2);

        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < TEST_NUM; ++i){
            lSum = std::inner_product(lVector1.cbegin(), lVector1.cend(), lVector2.cbegin(), 0.0);
        }
        auto lEnd = std::chrono::system_clock::now();
        lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < TEST_NUM; ++i){
            lSumOptSIMDV = pml::inner_product_SIMD(lVector1, lVector2, 0.0);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        std::array<double, TEST_ARRAY_SIZE> lArr1;
        std::array<double, TEST_ARRAY_SIZE> lArr2;
        lInitializer(lArr1, lArr2);
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < TEST_NUM; ++i){
            lSumOptSIMDA = pml::inner_product_SIMD(lArr1, lArr2, 0.0);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    outputResult(
        lSum, lSumOptSIMDV, lSumOptSIMDA,
        lElapsed, lOptSIMDVElapsed, lOptSIMDAElapsed);
}