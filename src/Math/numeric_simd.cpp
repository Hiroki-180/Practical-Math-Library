#include <PML/Math/numeric_simd.h>

#include <intrin.h>

PML_MATH_NS_BEGIN

namespace simd {

    namespace detail {

        double accumulate_aligned_AVX(
            const double* inA,
            std::size_t inSize)
        {
            __m128d lSum128 = _mm_setzero_pd();

            for (std::size_t i = 0;i < inSize;i += 4)
            {
                // lA256 = ( A[i],  A[i+1],  A[i+2],  A[i+3] )
                __m256d lA256 = _mm256_load_pd(&inA[i]);

                // hiDual = ( A[i], A[i+1] )
                const __m128d hiDual = _mm256_extractf128_pd(lA256, 1);
                // loDual = ( A[i+2], A[i+3] )
                const __m128d loDual = _mm256_castpd256_pd128(lA256);

                // sumQuad = ( A[i]+A[i+2], A[i+1]+A[i+3] )
                const __m128d sumDual = _mm_add_pd(loDual, hiDual);

                // lSum128 = ( A[0]+...+A[i]+A[i+2], A[1]+...+A[i+1]+A[i+3] );
                lSum128 = _mm_add_pd(lSum128, sumDual);
            }

            PML_STATIC_ALLIGN(32) double lPartialSum[2] = { 0 };
            _mm_store_pd(lPartialSum, lSum128);
            auto lSum = lPartialSum[0] + lPartialSum[1];

            const std::size_t lOffset = (inSize - inSize % 4);
            for (std::size_t i = lOffset;i < inSize; ++i)
            {
                lSum += inA[i];
            }

            return lSum;
        }

        double inner_prod_aligned_AVX(
            const double* inA,
            const double* inB,
            std::size_t inSize)
        {
            __m256d lSum256 = _mm256_setzero_pd();

            for (std::size_t i = 0;i < inSize;i += 4)
            {
                __m256d lA256 = _mm256_load_pd(&inA[i]);
                __m256d lB256 = _mm256_load_pd(&inB[i]);

                lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lA256, lB256));
            }

            PML_STATIC_ALLIGN(32) double lPartialSum[4] = { 0 };
            _mm256_store_pd(lPartialSum, lSum256);
            auto lSum = lPartialSum[0] + lPartialSum[1] + lPartialSum[2] + lPartialSum[3];

            const std::size_t lOffset = (inSize - inSize % 4);
            for (std::size_t i = lOffset;i < inSize; ++i)
            {
                lSum += inA[i] * inB[i];
            }

            return lSum;
        }
    }

    double inner_prod_AVX(
        const double* inA,
        const double* inB,
        std::size_t inSize)
    {
        __m256d lSum256 = _mm256_setzero_pd();

        for (std::size_t i = 0;i < inSize;i += 4)
        {
            __m256d lA256 = _mm256_loadu_pd(&inA[i]);
            __m256d lB256 = _mm256_loadu_pd(&inB[i]);

            lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lA256, lB256));
        }

        PML_STATIC_ALLIGN(32) double lPartialSum[4] = { 0 };
        _mm256_store_pd(lPartialSum, lSum256);
        auto lSum = lPartialSum[0] + lPartialSum[1] + lPartialSum[2] + lPartialSum[3];

        const std::size_t lOffset = (inSize - inSize % 4);
        for (std::size_t i = lOffset;i < inSize; ++i)
        {
            lSum += inA[i] * inB[i];
        }

        return lSum;
    }

    double accumulate_AVX(
        const double* inA,
        std::size_t inSize)
    {
        __m128d lSum128 = _mm_setzero_pd();

        for (std::size_t i = 0;i < inSize;i += 4)
        {
            // lA256 = ( A[i],  A[i+1],  A[i+2],  A[i+3] )
            __m256d lA256 = _mm256_loadu_pd(&inA[i]);

            // hiDual = ( A[i], A[i+1] )
            const __m128d hiDual = _mm256_extractf128_pd(lA256, 1);
            // loDual = ( A[i+2], A[i+3] )
            const __m128d loDual = _mm256_castpd256_pd128(lA256);

            // sumQuad = ( A[i]+A[i+2], A[i+1]+A[i+3] )
            const __m128d sumDual = _mm_add_pd(loDual, hiDual);

            // lSum128 = ( A[0]+...+A[i]+A[i+2], A[1]+...+A[i+1]+A[i+3] );
            lSum128 = _mm_add_pd(lSum128, sumDual);
        }

        PML_STATIC_ALLIGN(32) double lPartialSum[2] = { 0 };
        _mm_store_pd(lPartialSum, lSum128);
        auto lSum = lPartialSum[0] + lPartialSum[1];

        const std::size_t lOffset = (inSize - inSize % 4);
        for (std::size_t i = lOffset;i < inSize; ++i)
        {
            lSum += inA[i];
        }

        return lSum;
    }
}

PML_MATH_NS_END