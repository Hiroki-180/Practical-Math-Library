#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Core/CPUDispatcher.h>

#include <chrono>

TEST(TestNumericSIMD, InnerProd_AVX)
{
    if (!pml::CPUDispatcher::isAVX()
            && !pml::CPUDispatcher::isAVX2())
    {
        return;
    }

    const std::size_t lSize = 100;
    auto lAArray1 = pml::createAlignedArray<double>(lSize, 32); // ToDo: automatic detection of alignment size
    auto lAArray2 = pml::createAlignedArray<double>(lSize, 32); // ToDo: automatic detection of alignment size

    for (auto i = 0U;i < lSize;++i)
    {
        lAArray1[i] = 1.0;
        lAArray2[i] = i;
    }

    const auto lTestNum
#ifdef NDEBUG
        = 500000;
#else
        = 10000;
#endif

    // naive calculation.
    auto lInnerProd = 0.0;
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0;i < lTestNum;++i)
    {
        lInnerProd = 0.0;
        for (auto j = 0;j < lSize;++j)
        {
            lInnerProd += lAArray1[j] * lAArray2[j];
        }
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    // calculation using SIMD with std::vector
    auto lInnerProdSIMD = 0.0;
    long long lSIMDElapsed(0);
    {
        std::vector<double> lVector1(lSize);
        std::vector<double> lVector2(lSize);

        for (auto i = 0U;i < lSize;++i)
        {
            lVector1[i] = 1.0;
            lVector2[i] = i;
        }

        const auto lStart = std::chrono::system_clock::now();
        for (auto i = 0;i < lTestNum;++i)
        {
            lInnerProdSIMD = 0.0;
            lInnerProdSIMD = pml::inner_prod_AVX(lVector1, lVector2);
        }
        const auto lEnd = std::chrono::system_clock::now();
        lSIMDElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    // calculation using SIMD with aligned array
    auto lInnerProdSIMDAligned = 0.0;
    const auto lSIMDStart = std::chrono::system_clock::now();
    for (auto i = 0;i < lTestNum;++i)
    {
        lInnerProdSIMDAligned = 0.0;
        lInnerProdSIMDAligned = pml::inner_prod_aligned_AVX(lAArray1, lAArray2, lSize);
    }
    const auto lSIMDEnd = std::chrono::system_clock::now();
    const auto lSIMDElapsedAligned = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();

    EXPECT_EQ(lInnerProd, lInnerProdSIMDAligned);
    EXPECT_EQ(lInnerProd, lInnerProdSIMD);

    std::cout
#ifdef NDEBUG
        << "---Release Mode---\n"
#else
        << "---Debud Mode---\n"
#endif
        << lTestNum << "-times calculation,\n"
        << "Naive:" << lElapsed     << "[msec],\n"
        << "SIMD:"  << lSIMDElapsed << "[msec],\n"
        << "Aligned SIMD:" << lSIMDElapsedAligned << "[msec].\n";

#ifdef NDEBUG
    EXPECT_LT(lSIMDElapsed,        lElapsed);
    EXPECT_LT(lSIMDElapsedAligned, lElapsed);
    EXPECT_LE(lSIMDElapsedAligned, lSIMDElapsed);
#endif
}

TEST(TestNumericSIMD, Accumulate_AVX)
{
    if (!pml::CPUDispatcher::isAVX()
            && !pml::CPUDispatcher::isAVX2())
    {
        return;
    }

    const std::size_t lSize = 100;
    auto lAArray = pml::createAlignedArray<double>(lSize, 32); // ToDo: automatic detection of alignment size

    for (auto i = 0U;i < lSize;++i)
    {
        lAArray[i] = i;
    }

    const auto lTestNum
#ifdef NDEBUG
        = 500000;
#else
        = 10000;
#endif

    // naive calculation.
    auto lSum = 0.0;
    const auto lStart = std::chrono::system_clock::now();
    for (auto i = 0;i < lTestNum;++i)
    {
        lSum = 0.0;
        for (auto j = 0;j < lSize;++j)
        {
            lSum += lAArray[j];
        }
    }
    const auto lEnd = std::chrono::system_clock::now();
    const auto lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();

    // calculation using SIMD with std::vector
    auto lSumSIMD = 0.0;
    long long lSIMDElapsed(0);
    {
        std::vector<double> lVector(lSize);

        for (auto i = 0U;i < lSize;++i)
        {
            lVector[i] = i;
        }

        const auto lStart = std::chrono::system_clock::now();
        for (auto i = 0;i < lTestNum;++i)
        {
            lSumSIMD = 0.0;
            lSumSIMD = pml::accumulate_AVX(lVector);
        }
        const auto lEnd = std::chrono::system_clock::now();
        lSIMDElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
    }

    // calculation using SIMD with aligned array
    auto lSumSIMDAligned = 0.0;
    const auto lSIMDStart = std::chrono::system_clock::now();
    for (auto i = 0;i < lTestNum;++i)
    {
        lSumSIMDAligned = 0.0;
        lSumSIMDAligned = pml::accumulate_aligned_AVX(lAArray, lSize);
    }
    const auto lSIMDEnd = std::chrono::system_clock::now();
    const auto lSIMDElapsedAligned = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();

    EXPECT_EQ(lSum, lSumSIMD);
    EXPECT_EQ(lSum, lSumSIMDAligned);

    std::cout
#ifdef NDEBUG
        << "---Release Mode---\n"
#else
        << "---Debud Mode---\n"
#endif
        << lTestNum << "-times calculation,\n"
        << "Naive:" << lElapsed << "[msec],\n"
        << "SIMD:" << lSIMDElapsed << "[msec],\n"
        << "Aligned SIMD:" << lSIMDElapsedAligned << "[msec].\n";

#ifdef NDEBUG
    EXPECT_LT(lSIMDElapsed, lElapsed);
    EXPECT_LT(lSIMDElapsedAligned, lElapsed);
#endif
}