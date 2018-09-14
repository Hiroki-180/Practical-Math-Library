#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/inner_prod.h>

namespace pml {

    double inner_prod_AVX(
        const double* inA,
        const double* inB,
        std::size_t inSize)
    {
        __m256d lSum256 = _mm256_setzero_pd();

        for (std::size_t i = 0; i < inSize; i += 4)
        {
            __m256d lA256 = _mm256_loadu_pd(&inA[i]);
            __m256d lB256 = _mm256_loadu_pd(&inB[i]);

            lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lA256, lB256));
        }

        PML_STATIC_ALLIGN(32) double lPartialSum[4] = { 0 };
        _mm256_store_pd(lPartialSum, lSum256);
        auto lSum = lPartialSum[0] + lPartialSum[1] + lPartialSum[2] + lPartialSum[3];

        const std::size_t lOffset = (inSize - inSize % 4);
        for (std::size_t i = lOffset; i < inSize; ++i)
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


    namespace aligned {
        namespace detail {

            double inner_prod_AVX(
                const double* inA,
                const double* inB,
                std::size_t inSize)
            {
                __m256d lSum256 = _mm256_setzero_pd();

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 0; i < lUnrollEnd; i += 8)
                {
                    const __m256d lAF256 = _mm256_load_pd(&inA[i]);
                    const __m256d lAB256 = _mm256_load_pd(&inA[i + 4]);

                    const __m256d lBF256 = _mm256_load_pd(&inB[i]);
                    const __m256d lBB256 = _mm256_load_pd(&inB[i + 4]);

                    lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lAF256, lBF256));
                    lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lAB256, lBB256));
                }

                const std::size_t l256End = (inSize - (inSize & 3));
                if (l256End != lUnrollEnd)
                {
                    const __m256d lA256 = _mm256_load_pd(&inA[lUnrollEnd]);
                    const __m256d lB256 = _mm256_load_pd(&inB[lUnrollEnd]);

                    lSum256 = _mm256_add_pd(lSum256, _mm256_mul_pd(lA256, lB256));
                }

                const __m128d hiDual = _mm256_extractf128_pd(lSum256, 1);
                const __m128d loDual = _mm256_castpd256_pd128(lSum256);
                const __m128d lSum128 = _mm_add_pd(loDual, hiDual);

                PML_STATIC_ALLIGN(16) double lPartialSum[2] = { 0 };
                _mm_store_pd(lPartialSum, lSum128);
                auto lSum = lPartialSum[0] + lPartialSum[1];

                for (std::size_t i = l256End; i < inSize; ++i)
                {
                    lSum += inA[i] * inB[i];
                }

                return lSum;
            }
        }

        double inner_prod_AVX(
            const alarray<double>& inA,
            const alarray<double>& inB,
            std::size_t inSize)
        {
            return detail::inner_prod_AVX(inA.get(), inB.get(), inSize);
        }

        double inner_prod_AVX(
            const alvector32<double>& inA,
            const alvector32<double>& inB)
        {
            return detail::inner_prod_AVX(inA.data(), inB.data(), inA.size());
        }

    } // aligned
} // pml