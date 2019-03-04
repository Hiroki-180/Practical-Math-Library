#ifndef MATH_NUMERIC_SIMD_H
#define MATH_NUMERIC_SIMD_H

/**
* @file
* public header provided by PML.
*
* @brief
* numeric manipulation implemented by SIMD operations.
*/

#include <PML/Core/aligned_allocator.h>
#include <PML/Core/CPUDispatcher.h>

#include <type_traits>
#include <numeric>

namespace pml {

    /**
    * @brief
    * Accelerated version of std::accumulate by SIMD.
    * If Allocator is quants::aligned_allocator, _mm256_load_pd is applied in data loading.
    * If not, then _mm256_loadu_pd is applied in data loading.
    *
    * @param[in] inA
    * Array of std::vector to sum.
    *
    * @param[in] inVal
    * Initial value of the sum.
    *
    * @return
    * Sum of the all elements of the input array, inVal + (inA[0] + inA[1] + ... + inA[inA.size()-1]).
    */
    template<typename Allocator>
    double accumulate_SIMD(const std::vector<double, Allocator>& inA, double inVal)
    {
        if (CPUDispatcher::isAVX())
        {
            if constexpr (std::is_same<Allocator, pml::aligned_allocator<double>>::value)
            {
                return inVal + detail::accumulate_AVX_Impl(
                    inA.data(), inA.size(),
                    [](auto* inArray) { return _mm256_load_pd(inArray); });
            }
            else {
                return inVal + detail::accumulate_AVX_Impl(
                    inA.data(), inA.size(),
                    [](auto* inArray) { return _mm256_loadu_pd(inArray); });
            }
        }

        return std::accumulate(inA.cbegin(), inA.cend(), inVal);
    }

    /**
    * @brief
    * Accelerated version of std::inner_product by automatically selected optimal SIMD.
    *
    * @param[in] inA
    * 1st array as std::vector.
    *
    * @param[in] inB
    * 2nd array as std::vector.
    *
    * @return
    * Inner product of the input arrays, inA[0]*inB[0] + inA[1]*inB[1] + ... + inA[inA.size()-1]*inB[inB.size()-1].
    */
    template<typename Allocator>
    double inner_product_SIMD(
        const std::vector<double, Allocator>& inA,
        const std::vector<double, Allocator>& inB,
        double inVal)
    {
        if (CPUDispatcher::isAVX())
        {
            if constexpr (std::is_same<Allocator, pml::aligned_allocator<double>>::value)
            {
                return inVal + detail::inner_product_AVX_Impl(
                    inA.data(), inB.data(), inA.size(),
                    [](auto* inArray) { return _mm256_load_pd(inArray); });
            }
            else {
                return inVal + detail::inner_product_AVX_Impl(
                    inA.data(), inB.data(), inA.size(),
                    [](auto* inArray) { return _mm256_loadu_pd(inArray); });
            }
        }

        return std::inner_product(inA.cbegin(), inA.cend(), inB.cbegin(), inVal);
    }

    namespace detail {

        template<typename L>
        double accumulate_AVX_Impl(
            const double* inA,
            std::size_t inSize,
            L inLoader)
        {
            __m256d lSum256 = _mm256_setzero_pd();

            const std::size_t lUnrollEnd = (inSize - (inSize & 7));
            for (std::size_t i = 0; i < lUnrollEnd; i += 8)
            {
                const __m256d lF256 = inLoader(&inA[i]);
                const __m256d lB256 = inLoader(&inA[i + 4]);

                lSum256 = _mm256_add_pd(lSum256, _mm256_add_pd(lF256, lB256));
            }

            const std::size_t l256End = (inSize - (inSize & 3));
            if (l256End != lUnrollEnd) {
                lSum256 = _mm256_add_pd(lSum256, inLoader(&inA[lUnrollEnd]));
            }

            const __m128d hiDual = _mm256_extractf128_pd(lSum256, 1);
            const __m128d loDual = _mm256_castpd256_pd128(lSum256);
            const __m128d lSum128 = _mm_add_pd(loDual, hiDual);

            alignas(16) double lPartialSum[2] = { 0 };
            _mm_store_pd(lPartialSum, lSum128);
            auto lSum = lPartialSum[0] + lPartialSum[1];

            for (std::size_t i = l256End; i < inSize; ++i) {
                lSum += inA[i];
            }

            return lSum;
        }

        template<typename L>
        double inner_product_AVX_Impl(
            const double* inA,
            const double* inB,
            std::size_t inSize,
            L inLoader)
        {
            __m256d lSum256 = _mm256_setzero_pd();

            const std::size_t lUnrollEnd = (inSize - (inSize & 7));
            for (std::size_t i = 0; i < lUnrollEnd; i += 8)
            {
                const __m256d lAF256 = inLoader(&inA[i]);
                const __m256d lAB256 = inLoader(&inA[i + 4]);

                const __m256d lBF256 = inLoader(&inB[i]);
                const __m256d lBB256 = inLoader(&inB[i + 4]);

                lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lAF256, lBF256));
                lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lAB256, lBB256));
            }

            const std::size_t l256End = (inSize - (inSize & 3));
            if (l256End != lUnrollEnd)
            {
                const __m256d lA256 = inLoader(&inA[lUnrollEnd]);
                const __m256d lB256 = inLoader(&inB[lUnrollEnd]);

                lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lA256, lB256));
            }

            const __m128d hiDual = _mm256_extractf128_pd(lSum256, 1);
            const __m128d loDual = _mm256_castpd256_pd128(lSum256);
            const __m128d lSum128 = _mm_add_pd(loDual, hiDual);

            alignas(16) double lPartialSum[2] = { 0 };
            _mm_store_pd(lPartialSum, lSum128);
            auto lSum = lPartialSum[0] + lPartialSum[1];

            for (std::size_t i = l256End; i < inSize; ++i) {
                lSum += inA[i] * inB[i];
            }

            return lSum;
        }

    } // detail
} // pml

#endif