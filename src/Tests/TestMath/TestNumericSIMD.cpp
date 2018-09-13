#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Core/CPUDispatcher.h>
#include <chrono>
#include <cmath>

namespace {

	void escapeIfAVSIsNotSupported()
	{
		if (!pml::CPUDispatcher::isAVX())
		{
			std::cout << "AVX is not supported." << std::endl;
			return;
		}
	}

	void outputResult(
		std::size_t inTestNum,
		long long inElapsedTimeNaive,
		long long inElapsedTimeSIMDVector,
		long long inElapsedTimeSIMDAlignedArray,
		long long inElapsedTimeSIMDAlignedVector)
	{
		std::cout
#ifdef NDEBUG
			<< "---Release Mode---\n"
#else
			<< "---Debud Mode---\n"
#endif
			<< inTestNum << "-times calculation,\n"
			<< "Naive:" << inElapsedTimeNaive << "[msec],\n"
			<< "Vector SIMD:" << inElapsedTimeSIMDVector << "[msec],\n"
			<< "Aligned Array SIMD:" << inElapsedTimeSIMDAlignedArray << "[msec].\n"
			<< "Aligned Vector SIMD:" << inElapsedTimeSIMDAlignedVector << "[msec].\n";

#ifdef NDEBUG
		std::cout.precision(16);
		EXPECT_LT(inElapsedTimeSIMDVector, inElapsedTimeNaive);
		EXPECT_LT(inElapsedTimeSIMDAlignedArray, inElapsedTimeNaive);
		EXPECT_LE(inElapsedTimeSIMDAlignedArray, inElapsedTimeSIMDVector);
		EXPECT_LT(inElapsedTimeSIMDAlignedVector, inElapsedTimeNaive);
		EXPECT_LE(inElapsedTimeSIMDAlignedVector, inElapsedTimeSIMDVector);
#endif
	}
}

TEST(TestNumericSIMD, Accumulate_AVX)
{
	escapeIfAVSIsNotSupported();

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

    // naive calculation.
    auto lSum = 0.0;
	long long lElapsed = 0;
	{
		auto lAArray = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray);

		const auto lStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			lSum = 0.0;
			for (auto j = 0; j < lSize; ++j)
			{
				lSum += lAArray[j];
			}
		}
		const auto lEnd = std::chrono::system_clock::now();
		lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
	}

    // calculation using SIMD with std::vector
    auto lSumSIMD = 0.0;
    long long lSIMDElapsed = 0;
    {
        std::vector<double> lVector(lSize);
		lInitializer(lVector);

        const auto lStartSIMD = std::chrono::system_clock::now();
        for (auto i = 0;i < lTestNum;++i)
        {
            lSumSIMD = 0.0;
            lSumSIMD = pml::accumulate_AVX(lVector);
        }
        const auto lEndSIMD = std::chrono::system_clock::now();
        lSIMDElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEndSIMD - lStartSIMD).count();
    }

    // calculation using SIMD with aligned array
    auto lSumSIMDAligned = 0.0;
	long long lSIMDElapsedAligned = 0;
	{
		auto lAArray = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray);

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			lSumSIMDAligned = 0.0;
			lSumSIMDAligned = pml::aligned::accumulate_AVX(lAArray, lSize);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAligned = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}

	// calculation using SIMD with aligned vector
	auto lSumSIMDAlignedV = 0.0;
	long long lSIMDElapsedAlignedV = 0;
	{
		auto lAVector = pml::aligned::alvector32<double>(lSize);
		lInitializer(lAVector);

		const auto lSIMDVStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			lSumSIMDAlignedV = 0.0;
			lSumSIMDAlignedV = pml::aligned::accumulate_AVX(lAVector);
		}
		const auto lSIMDVEnd = std::chrono::system_clock::now();
		lSIMDElapsedAlignedV = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDVEnd - lSIMDVStart).count();
	}

    EXPECT_EQ(lSum, lSumSIMD);
    EXPECT_EQ(lSum, lSumSIMDAligned);
	EXPECT_EQ(lSum, lSumSIMDAlignedV);

	outputResult(lTestNum, lElapsed, lSIMDElapsed, lSIMDElapsedAligned, lSIMDElapsedAlignedV);
}

TEST(TestNumericSIMD, InnerProd_AVX)
{
	escapeIfAVSIsNotSupported();

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
	
    // naive calculation.
    auto lInnerProd = 0.0;
	long long lElapsed = 0;
	{
		auto lAArray1 = pml::aligned::createArray<double>(lSize, 32);
		auto lAArray2 = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray1, lAArray2);

		const auto lStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			lInnerProd = 0.0;
			for (auto j = 0; j < lSize; ++j)
			{
				lInnerProd += lAArray1[j] * lAArray2[j];
			}
		}
		const auto lEnd = std::chrono::system_clock::now();
		lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
	}

    // calculation using SIMD with std::vector
    auto lInnerProdSIMD = 0.0;
    long long lSIMDElapsed = 0;
    {
        std::vector<double> lVector1(lSize);
        std::vector<double> lVector2(lSize);
		lInitializer(lVector1, lVector2);

        const auto lStartSIMD = std::chrono::system_clock::now();
        for (auto i = 0;i < lTestNum;++i)
        {
            lInnerProdSIMD = 0.0;
            lInnerProdSIMD = pml::inner_prod_AVX(lVector1, lVector2);
        }
        const auto lEndSIMD = std::chrono::system_clock::now();
        lSIMDElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEndSIMD - lStartSIMD).count();
    }

    // calculation using SIMD with aligned array
    auto lInnerProdSIMDAligned = 0.0;
	long long lSIMDElapsedAligned = 0;
	{
		auto lAArray1 = pml::aligned::createArray<double>(lSize, 32);
		auto lAArray2 = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray1, lAArray2);

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			lInnerProdSIMDAligned = 0.0;
			lInnerProdSIMDAligned = pml::aligned::inner_prod_AVX(lAArray1, lAArray2, lSize);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAligned = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}

	// calculation using SIMD with aligned vector
	auto lInnerProdSIMDAlignedV = 0.0;
	long long lSIMDElapsedAlignedV = 0;
	{
		auto lAVector1 = pml::aligned::alvector32<double>(lSize);
		auto lAVector2 = pml::aligned::alvector32<double>(lSize);
		lInitializer(lAVector1, lAVector2);

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			lInnerProdSIMDAlignedV = 0.0;
			lInnerProdSIMDAlignedV = pml::aligned::inner_prod_AVX(lAVector1, lAVector2);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAlignedV = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}

    EXPECT_EQ(lInnerProd, lInnerProdSIMD);
	EXPECT_EQ(lInnerProd, lInnerProdSIMDAligned);
	EXPECT_EQ(lInnerProd, lInnerProdSIMDAlignedV);

	outputResult(lTestNum, lElapsed, lSIMDElapsed, lSIMDElapsedAligned, lSIMDElapsedAlignedV);
}

TEST(TestNumericSIMD, adjacent_divide_aligned_AVX)
{
	escapeIfAVSIsNotSupported();

	const auto lTestNum
#ifdef NDEBUG
		= 100000;
#else
		= 5000;
#endif
	
	const std::size_t lSize = 100;
	auto lInitializer = [lSize](auto& inArray)->void
	{
		for (auto i = 0U; i < lSize; ++i)
		{
			inArray[i] = std::max(std::cos(static_cast<double>(i)), std::sin(static_cast<double>(i)));
		}
	};
	
    // naive calculation.
    auto lAArrayAns = pml::aligned::createArray<double>(lSize, 32);
	long long lElapsed = 0;
	{
		auto lAArray = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray);

		const auto lStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			for (auto j = 0; j < (lSize - 1); ++j)
			{
				lAArrayAns[j] = lAArray[j] / lAArray[j + 1] - 1.0;
			}

			lAArrayAns[lSize - 1] = lAArray[lSize - 1];
		}
		const auto lEnd = std::chrono::system_clock::now();
		lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
	}

    // calculation using SIMD with std::vector
    std::vector<double> lVectorAnsSIMD(lSize);
    long long lSIMDElapsed = 0;
    {
        std::vector<double> lVector(lSize);
		lInitializer(lVector);

        const auto lStartSIMD = std::chrono::system_clock::now();
        for (auto i = 0;i < lTestNum;++i)
        {
            pml::adjacent_divide_AVX(lVector, -1.0, lVectorAnsSIMD);
        }
        const auto lEndSIMD = std::chrono::system_clock::now();
        lSIMDElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEndSIMD - lStartSIMD).count();
    }

    // calculation using SIMD with aligned array
    auto lAArrayAnsSIMDAligned = pml::aligned::createArray<double>(lSize, 32);
	long long lSIMDElapsedAligned = 0;
	{
		auto lAArray = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray);

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			pml::aligned::adjacent_divide_AVX(lAArray, -1.0, lAArrayAnsSIMDAligned, lSize);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAligned = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}

	// calculation using SIMD with aligned vector
	auto lAVectorAnsSIMDAlignedV = pml::aligned::alvector32<double>(lSize);
	long long lSIMDElapsedAlignedV = 0;
	{
		auto lAVector = pml::aligned::alvector32<double>(lSize);
		lInitializer(lAVector);

		for (auto i = 0U; i < lSize; ++i)
		{
			lAVector[i] = std::max(std::cos(static_cast<double>(i)), std::sin(static_cast<double>(i)));
		}

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			pml::aligned::adjacent_divide_AVX(lAVector, -1.0, lAVectorAnsSIMDAlignedV);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAlignedV = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}
	
	for (auto i = 0;i < lSize;++i)
    {
        EXPECT_NEAR(lAArrayAns[i], lVectorAnsSIMD[i], 1.0E-14);
        EXPECT_DOUBLE_EQ(lAArrayAns[i], lAArrayAnsSIMDAligned[i]) << i <<std::endl;
		EXPECT_DOUBLE_EQ(lAArrayAns[i], lAVectorAnsSIMDAlignedV[i]) << i << std::endl;
	}

	outputResult(lTestNum, lElapsed, lSIMDElapsed, lSIMDElapsedAligned, lSIMDElapsedAlignedV);
}

TEST(TestNumericSIMD, positive_difference_AVX)
{
	escapeIfAVSIsNotSupported();

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
			inArray2[i] = std::cos(static_cast<double>(i));
		}
	};

    // naive calculation.
    auto lAArrayAns = pml::aligned::createArray<double>(lSize, 32);
	long long lElapsed = 0;
	{
		auto lAArray1 = pml::aligned::createArray<double>(lSize, 32);
		auto lAArray2 = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray1, lAArray2);

		const auto lStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			for (auto j = 0; j < lSize; ++j)
			{
				lAArrayAns[j] = std::max(lAArray1[j] - lAArray2[j], 0.0);
			}
		}
		const auto lEnd = std::chrono::system_clock::now();
		lElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEnd - lStart).count();
	}

    // calculation using SIMD with std::vector
    std::vector<double> lVectorAnsSIMD(lSize);
    long long lSIMDElapsed = 0;
    {
        std::vector<double> lVector1(lSize);
        std::vector<double> lVector2(lSize);
		lInitializer(lVector1, lVector2);

        for (auto i = 0U;i < lSize;++i)
        {
            lVector1[i] = std::sin(static_cast<double>(i));
            lVector2[i] = std::cos(static_cast<double>(i));
        }

        const auto lStartSIMD = std::chrono::system_clock::now();
        for (auto i = 0;i < lTestNum;++i)
        {
            pml::positive_difference_AVX(lVector1, lVector2, lVectorAnsSIMD);
        }
        const auto lEndSIMD = std::chrono::system_clock::now();
        lSIMDElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(lEndSIMD - lStartSIMD).count();
    }

    // calculation using SIMD with aligned array
    auto lAArrayAnsSIMDAligned = pml::aligned::createArray<double>(lSize, 32);
	long long lSIMDElapsedAligned = 0;
	{
		auto lAArray1 = pml::aligned::createArray<double>(lSize, 32);
		auto lAArray2 = pml::aligned::createArray<double>(lSize, 32);
		lInitializer(lAArray1, lAArray2);

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			pml::aligned::positive_difference_AVX(lAArray1, lAArray2, lAArrayAnsSIMDAligned, lSize);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAligned = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}

	// calculation using SIMD with aligned vector
	auto lAVectorAnsSIMDAlignedV = pml::aligned::alvector32<double>(lSize);
	long long lSIMDElapsedAlignedV = 0;
	{
		auto lAVector1 = pml::aligned::alvector32<double>(lSize);
		auto lAVector2 = pml::aligned::alvector32<double>(lSize);
		lInitializer(lAVector1, lAVector2);

		const auto lSIMDStart = std::chrono::system_clock::now();
		for (auto i = 0; i < lTestNum; ++i)
		{
			pml::aligned::positive_difference_AVX(lAVector1, lAVector2, lAVectorAnsSIMDAlignedV);
		}
		const auto lSIMDEnd = std::chrono::system_clock::now();
		lSIMDElapsedAlignedV = std::chrono::duration_cast<std::chrono::milliseconds>(lSIMDEnd - lSIMDStart).count();
	}
	
	for (auto i = 0;i < lSize;++i)
    {
        EXPECT_NEAR(lAArrayAns[i], lVectorAnsSIMD[i], 1.0E-14);
        EXPECT_DOUBLE_EQ(lAArrayAns[i], lAArrayAnsSIMDAligned[i]);
		EXPECT_DOUBLE_EQ(lAArrayAns[i], lAVectorAnsSIMDAlignedV[i]);
	}

	outputResult(lTestNum, lElapsed, lSIMDElapsed, lSIMDElapsedAligned, lSIMDElapsedAlignedV);
}
