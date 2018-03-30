#include <PML/Math/numeric_simd.h>

#include <intrin.h>

namespace pml {

    namespace math {
        namespace numeric_simd {

            double accumulate_aligned_AVX(
                const double* inA,
                std::size_t inSize)
            {
                __m256d lSum256 = _mm256_setzero_pd();

                const std::size_t lUnrollEnd = 8 * (inSize / 8);
                for (std::size_t i = 0;i < lUnrollEnd;i += 8)
                {
                    const __m256d lF256 = _mm256_load_pd(&inA[i]);
                    const __m256d lB256 = _mm256_load_pd(&inA[i+4]);

                    lSum256 = _mm256_add_pd(lSum256, _mm256_add_pd(lF256, lB256));
                }

                const std::size_t l256End = lUnrollEnd + 4*((inSize - lUnrollEnd) / 4);
                if (l256End != lUnrollEnd)
                {
                    lSum256 = _mm256_add_pd(lSum256, _mm256_load_pd(&inA[lUnrollEnd]));
                }

                const __m128d hiDual = _mm256_extractf128_pd(lSum256, 1);
                const __m128d loDual = _mm256_castpd256_pd128(lSum256);
                const __m128d lSum128 = _mm_add_pd(loDual, hiDual);

                PML_STATIC_ALLIGN(32) double lPartialSum[2] = { 0 };
                _mm_store_pd(lPartialSum, lSum128);
                auto lSum = lPartialSum[0] + lPartialSum[1];

                for (std::size_t i = l256End;i < inSize; ++i)
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
    }

    double accumulate_AVX(
        const double* inA,
        std::size_t inSize)
    {
        // Unrolling ver. should be slower than following naive code because of _mm256_loadu_pd.

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

    double accumulate_AVX(const std::vector<double>& inA)
    {
        return accumulate_AVX(inA.data(), inA.size());
    }

    double accumulate_aligned_AVX(
        const aligned_array<double>& inA,
        std::size_t inSize)
    {
        return math::numeric_simd::accumulate_aligned_AVX(inA.get(), inSize);
    }

    double accumulate_aligned_AVX(const align32_vector<double>& inA)
    {
        return math::numeric_simd::accumulate_aligned_AVX(inA.data(), inA.size());
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

    double inner_prod_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB)
    {
        return inner_prod_AVX(inA.data(), inB.data(), inA.size());
    }

    double inner_prod_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        std::size_t inSize)
    {
        return math::numeric_simd::inner_prod_aligned_AVX(inA.get(), inB.get(), inSize);
    }

    double inner_prod_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB)
    {
        return math::numeric_simd::inner_prod_aligned_AVX(inA.data(), inB.data(), inA.size());
    }

} // pml