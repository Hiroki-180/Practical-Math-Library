#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Math/numeric_simd/accumulate.h>
#include <PML/Math/numeric_simd/inner_product.h>
#include <PML/Math/numeric_simd/positive_difference.h>
#include <PML/Core/CPUDispatcher.h>
#include <numeric>
#include <chrono>
#include <cmath>

namespace {

    template<typename T>
    void outputResult(
        std::size_t inTestNum,
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
            << "Naive           :" << inElapsedTime         << "[msec],\n"
            << "Opt. SIMD Vector:" << inElapsedTimeOptSIMDV << "[msec],\n"
            << "AVX Vector      :" << inElapsedTimeAVXV     << "[msec].\n"
            << "Opt. SIMD Array :" << inElapsedTimeOptSIMDA << "[msec].\n"
            << "AVX Array       :" << inElapsedTimeAVXA     << "[msec].\n"
            << "Opt. SIMD Aligned Vector:" << inElapsedTimeOptSIMDAV << "[msec].\n"
            << "AVX Aligned Vector     :" << inElapsedTimeAVXAV      << "[msec].\n";

#ifdef NDEBUG
        EXPECT_LT(inElapsedTimeOptSIMDV,  inElapsedTime);
        EXPECT_LT(inElapsedTimeAVXV,      inElapsedTime);
        EXPECT_LT(inElapsedTimeOptSIMDA,  inElapsedTime);
        EXPECT_LT(inElapsedTimeAVXA,      inElapsedTime);
        EXPECT_LE(inElapsedTimeOptSIMDAV, inElapsedTime);
        EXPECT_LE(inElapsedTimeAVXAV,     inElapsedTime);
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

    const std::size_t lSize = 100;
	auto lInitializer = [lSize](auto& inArray)->void
	{
		for (auto i = 0U; i < lSize; ++i)
		{
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
            lSumOptSIMDV = pml::accumulate_SIMD(lVector);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumAVXV = pml::accumulate_AVX(lVector);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDA = pml::accumulate_SIMD(lVector.data(), lVector.size());
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumAVXA = pml::accumulate_AVX(lVector.data(), lVector.size());
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
        auto lAVector = pml::aligned::alvector<double>(lSize);
        lInitializer(lAVector);

        // calculation using automatically selected optimal SIMD with std::vector
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDAV = pml::aligned::accumulate_SIMD(lAVector);
        }
        auto lEnd = std::chrono::system_clock::now();
        lOptSIMDAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumAVXAV = pml::aligned::accumulate_AVX(lAVector);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

	outputResult(lTestNum,
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

    const std::size_t lSize = 100;
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
            lSumOptSIMDV = pml::inner_product_SIMD(lVector1, lVector2);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumAVXV = pml::inner_product_AVX(lVector1, lVector2);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDA = pml::inner_product_SIMD(lVector1.data(), lVector2.data(), lVector1.size());
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumAVXA = pml::inner_product_AVX(lVector1.data(), lVector2.data(), lVector1.size());
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
        auto lAVector1 = pml::aligned::alvector<double>(lSize);
        auto lAVector2 = pml::aligned::alvector<double>(lSize);
        lInitializer(lAVector1, lAVector2);

        // calculation using automatically selected optimal SIMD with std::vector
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumOptSIMDAV = pml::aligned::inner_product_SIMD(lAVector1, lAVector2);
        }
        auto lEnd = std::chrono::system_clock::now();
        lOptSIMDAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            lSumAVXAV = pml::aligned::inner_product_AVX(lAVector1, lAVector2);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    outputResult(lTestNum,
        lSum, lSumOptSIMDV, lSumAVXV, lSumOptSIMDA, lSumAVXA, lSumOptSIMDAV, lSumAVXAV,
        lElapsed, lOptSIMDVElapsed, lAVXVElapsed, lOptSIMDAElapsed, lAVXAElapsed, lOptSIMDAVElapsed, lAVXAVElapsed);
}

TEST(TestNumericSIMD, positive_difference)
{
    const auto lTestNum
#ifdef NDEBUG
        = 500000;
#else
        = 2000;
#endif

    const std::size_t lSize = 100;
    auto lInitializer = [lSize](auto& inArray1, auto& inArray2)->void
    {
        for (auto i = 0U; i < lSize; ++i)
        {
            inArray1[i] = std::sin(static_cast<double>(i));
            inArray2[i] = std::log(static_cast<double>(i));
        }
    };

    // In case of the memory alignent is not guaranteed.
    // STL.
    auto lAns = std::vector<double>(lSize);
    long long lElapsed = 0;
    // automatically selected optimal SIMD with std::vector.
    auto lAnsOptSIMDV = std::vector<double>(lSize);
    long long lOptSIMDVElapsed = 0;
    // AVX with std::vector.
    auto lAnsAVXV = std::vector<double>(lSize);
    long long lAVXVElapsed = 0;
    // automatically selected optimal SIMD with array.
    auto lAnsOptSIMDA = std::vector<double>(lSize);
    long long lOptSIMDAElapsed = 0;
    // AVX with array.
    auto lAnsAVXA = std::vector<double>(lSize);
    long long lAVXAElapsed = 0;
    {
        std::vector<double> lVector1(lSize);
        std::vector<double> lVector2(lSize);
        lInitializer(lVector1, lVector2);

        // naive.
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            for (std::size_t j = 0; j < lSize; ++j)
            {
                lAns[j] = std::max(lVector1[j] - lVector2[j], 0.0);
            }
        }
        auto lEnd = std::chrono::system_clock::now();
        lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            pml::positive_difference_SIMD(lVector1, lVector2, lAnsOptSIMDV);
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            pml::positive_difference_AVX(lVector1, lVector2, lAnsAVXV);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using automatically selected optimal SIMD with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            pml::positive_difference_SIMD(lVector1.data(), lVector2.data(), lAnsOptSIMDA.data(), lVector1.size());
        }
        lEnd = std::chrono::system_clock::now();
        lOptSIMDAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

        // calculation using AVX with array.
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            pml::positive_difference_AVX(lVector1.data(), lVector2.data(), lAnsAVXA.data(), lVector1.size());
        }
        lEnd = std::chrono::system_clock::now();
        lAVXAElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    // In case of the memory alignent is guaranteed.
    // automatically selected optimal SIMD with aligned vector.
    auto lAnsOptSIMDAVAligned = pml::aligned::alvector<double>(lSize);
    auto lAnsOptSIMDAV = std::vector<double>(lSize);
    long long lOptSIMDAVElapsed = 0;
    // AVX with aligned vector.
    auto lAnsAVXAVAligned = pml::aligned::alvector<double>(lSize);
    auto lAnsAVXAV = std::vector<double>(lSize);
    long long lAVXAVElapsed = 0;
    {
        auto lAVector1 = pml::aligned::alvector<double>(lSize);
        auto lAVector2 = pml::aligned::alvector<double>(lSize);
        lInitializer(lAVector1, lAVector2);

        // calculation using automatically selected optimal SIMD with std::vector
        auto lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            pml::aligned::positive_difference_SIMD(lAVector1, lAVector2, lAnsOptSIMDAVAligned);
        }
        auto lEnd = std::chrono::system_clock::now();
        lOptSIMDAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
        lAnsOptSIMDAV.assign(lAnsOptSIMDAVAligned.cbegin(), lAnsOptSIMDAVAligned.cend());

        // calculation using AVX with std::vector
        lStart = std::chrono::system_clock::now();
        for (auto i = 0; i < lTestNum; ++i)
        {
            pml::aligned::positive_difference_AVX(lAVector1, lAVector2, lAnsAVXAVAligned);
        }
        lEnd = std::chrono::system_clock::now();
        lAVXAVElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
        lAnsAVXAV.assign(lAnsAVXAVAligned.cbegin(), lAnsAVXAVAligned.cend());
    }

    outputResult(lTestNum,
        lAns, lAnsOptSIMDV, lAnsAVXV, lAnsOptSIMDA, lAnsAVXA, lAnsOptSIMDAV, lAnsAVXAV,
        lElapsed, lOptSIMDVElapsed, lAVXVElapsed, lOptSIMDAElapsed, lAVXAElapsed, lOptSIMDAVElapsed, lAVXAVElapsed);
}