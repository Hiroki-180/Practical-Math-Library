#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Core/CPUDispatcher.h>
#include <numeric>
#include <chrono>
#include <cmath>

namespace {

    template<typename T>
    void outputResult(
        std::size_t inTestNum,
        std::size_t inSize,
        const T& inAns,
        const T& inOptSIMDVResult,
        const T& inAVXVResult,
        const T& inOptSIMDAResult,
        const T& inAVXAResult,
        const T& inOptSIMDAVResult,
        const T& inAVXAVResult,
        const long long inElapsedTime,
        const long long inElapsedTimeOptSIMDV,
        const long long inElapsedTimeAVXV,
        const long long inElapsedTimeOptSIMDA,
        const long long inElapsedTimeAVXA,
        const long long inElapsedTimeOptSIMDAV,
        const long long inElapsedTimeAVXAV)
    {
        EXPECT_EQ(inAns, inOptSIMDVResult);
        EXPECT_EQ(inAns, inAVXVResult);
        EXPECT_EQ(inAns, inOptSIMDAResult);
        EXPECT_EQ(inAns, inAVXAResult);
        EXPECT_EQ(inAns, inOptSIMDAVResult);
        EXPECT_EQ(inAns, inAVXAVResult);

        std::cout
#ifdef NDEBUG
            << "---Release Mode---\n"
#else
            << "---Debud Mode---\n"
#endif
            << inTestNum << "-times calculation,\n"
            << inSize << "-elements array,\n"
            << "Naive           :" << inElapsedTime         << "[msec],\n"
            << "Opt. SIMD Vector:" << inElapsedTimeOptSIMDV << "[msec],\n"
            << "AVX Vector      :" << inElapsedTimeAVXV     << "[msec].\n"
            << "Opt. SIMD Array :" << inElapsedTimeOptSIMDA << "[msec].\n"
            << "AVX Array       :" << inElapsedTimeAVXA     << "[msec].\n"
            << "Opt. SIMD Aligned Vector:" << inElapsedTimeOptSIMDAV << "[msec].\n"
            << "AVX Aligned Vector      :" << inElapsedTimeAVXAV     << "[msec].\n";

#ifdef NDEBUG
        EXPECT_LT(inElapsedTimeOptSIMDV,  inElapsedTime);
        EXPECT_LT(inElapsedTimeOptSIMDA,  inElapsedTime);
        EXPECT_LE(inElapsedTimeOptSIMDAV, inElapsedTime);
#endif
    }
}

TEST(TestNumericSIMD, accumulate)
{
	const auto lTestNum
#ifdef NDEBUG
		= 500000;
#else
		= 5000;
#endif

    const std::size_t lSize = 150;
	auto lInitializer = [lSize](auto& inArray)->void
	{
		for (auto i = 0U; i < lSize; ++i){
			inArray[i] = i;
		}
	};

    // In case of the memory alignent is not guaranteed.
    // STL.
    auto lSum = 0.0;
    long long lElapsed = 0;
    // automatically selected optimal SIMD with std::vector.
    auto lSumOptSIMDV = 0.0;
    long long lOptSIMDVElapsed = 0;
    // AVX with std::vector.
    auto lSumAVXV = 0.0;
    long long lAVXVElapsed = 0;
    // automatically selected optimal SIMD with array.
    auto lSumOptSIMDA = 0.0;
    long long lOptSIMDAElapsed = 0;
    // AVX with array.
    auto lSumAVXA = 0.0;
    long long lAVXAElapsed = 0;
    {
        std::vector<double> lVector(lSize);
        lInitializer(lVector);

        // STL.
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSum = std::accumulate(lVector.cbegin(), lVector.cend(), 0.0);
        }
        auto lEnd = std::chrono::system_clock::now();
        lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDV = pml::accumulate_SIMD(lVector, 0.0);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    // In case of the memory alignent is guaranteed.
    // automatically selected optimal SIMD with aligned vector.
    auto lSumOptSIMDAV = 0.0;
    long long lOptSIMDAVElapsed = 0;
    // AVX with aligned vector.
    auto lSumAVXAV = 0.0;
    long long lAVXAVElapsed = 0;
    {
        auto lAVector = std::vector<double, pml::aligned_allocator<double>>(lSize);
        lInitializer(lAVector);

        // calculation using automatically selected optimal SIMD with aligned vector
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDAV = pml::accumulate_SIMD(lAVector, 0.0);
        }
        auto lEnd = std::chrono::system_clock::now();
        lOptSIMDAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

	outputResult(
        lTestNum, lSize,
        lSum, lSumOptSIMDV, lSumAVXV, lSumOptSIMDA, lSumAVXA, lSumOptSIMDAV, lSumAVXAV,
        lElapsed, lOptSIMDVElapsed, lAVXVElapsed, lOptSIMDAElapsed, lAVXAElapsed, lOptSIMDAVElapsed, lAVXAVElapsed);
}

TEST(TestNumericSIMD, inner_product)
{
    const auto lTestNum
#ifdef NDEBUG
        = 500000;
#else
        = 5000;
#endif

    const std::size_t lSize = 150;
    auto lInitializer = [lSize](auto& inArray1, auto& inArray2)->void
    {
        for (auto i = 0U; i < lSize; ++i)
        {
            inArray1[i] = 1.0;
            inArray2[i] = i;
        }
    };

    // In case of the memory alignent is not guaranteed.
    // STL.
    auto lSum = 0.0;
    long long lElapsed = 0;
    // automatically selected optimal SIMD with std::vector.
    auto lSumOptSIMDV = 0.0;
    long long lOptSIMDVElapsed = 0;
    // AVX with std::vector.
    auto lSumAVXV = 0.0;
    long long lAVXVElapsed = 0;
    // automatically selected optimal SIMD with array.
    auto lSumOptSIMDA = 0.0;
    long long lOptSIMDAElapsed = 0;
    // AVX with array.
    auto lSumAVXA = 0.0;
    long long lAVXAElapsed = 0;
    {
        std::vector<double> lVector1(lSize);
        std::vector<double> lVector2(lSize);
        lInitializer(lVector1, lVector2);

        // STL.
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSum = std::inner_product(lVector1.cbegin(), lVector1.cend(), lVector2.cbegin(), 0.0);
        }
        auto lEnd = std::chrono::system_clock::now();
        lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDV = pml::inner_product_SIMD(lVector1, lVector2, 0.0);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            //lSumAVXV = pml::inner_product_AVX_vector(lVector1, lVector2);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            //lSumOptSIMDA = pml::inner_product_SIMD(lVector1.data(), lVector2.data(), lVector1.size());
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            //lSumAVXA = pml::inner_product_AVX_array(lVector1.data(), lVector2.data(), lVector1.size());
        }
        lEnd = std::chrono::system_clock::now();
        lAVXAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    // In case of the memory alignent is guaranteed.
    // automatically selected optimal SIMD with aligned vector.
    auto lSumOptSIMDAV = 0.0;
    long long lOptSIMDAVElapsed = 0;
    // AVX with aligned vector.
    auto lSumAVXAV = 0.0;
    long long lAVXAVElapsed = 0;
    {
        auto lAVector1 = std::vector<double, pml::aligned_allocator<double>>(lSize);
        auto lAVector2 = std::vector<double, pml::aligned_allocator<double>>(lSize);
        lInitializer(lAVector1, lAVector2);

        // calculation using automatically selected optimal SIMD with aligned vector
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDAV = pml::inner_product_SIMD(lAVector1, lAVector2, 0.0);
        }
        auto lEnd = std::chrono::system_clock::now();
        lOptSIMDAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with aligned vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            //lSumAVXAV = pml::aligned::inner_product_AVX_alvector(lAVector1, lAVector2);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    outputResult(
        lTestNum, lSize,
        lSum, lSumOptSIMDV, lSumAVXV, lSumOptSIMDA, lSumAVXA, lSumOptSIMDAV, lSumAVXAV,
        lElapsed, lOptSIMDVElapsed, lAVXVElapsed, lOptSIMDAElapsed, lAVXAElapsed, lOptSIMDAVElapsed, lAVXAVElapsed);
}