#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/accumulate.h>
#include <cassert>
#include <numeric>

namespace pml {

    namespace {

        template<typename T>
        double accumulate_AVX_Impl(
            const double* inA,
            std::size_t inSize,
            T inLoader)
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
            if (l256End != lUnrollEnd)
            {
                lSum256 = _mm256_add_pd(lSum256, inLoader(&inA[lUnrollEnd]));
            }

            const __m128d hiDual = _mm256_extractf128_pd(lSum256, 1);
            const __m128d loDual = _mm256_castpd256_pd128(lSum256);
            const __m128d lSum128 = _mm_add_pd(loDual, hiDual);

            PML_STATIC_ALLIGN(16) double lPartialSum[2] = { 0 };
            _mm_store_pd(lPartialSum, lSum128);
            auto lSum = lPartialSum[0] + lPartialSum[1];

            for (std::size_t i = l256End; i < inSize; ++i)
            {
                lSum += inA[i];
            }

            return lSum;
        }

        double accumulate_Naive_Impl(
            const double* inA,
            std::size_t inSize)
        {
            return std::accumulate(inA, inA + inSize, 0.0);
        }

    } // unnamed

    double accumulate_AVX_array(
        const double* inA,
        std::size_t inSize)
    {
        return accumulate_AVX_Impl(
            inA, inSize,
            [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    double accumulate_naive_array(
        const double* inA,
        std::size_t inSize)
    {
        return accumulate_Naive_Impl(inA, inSize);
    }

    double accumulate_AVX_vector(const std::vector<double>& inA)
    {
        return accumulate_AVX_Impl(
            inA.data(), inA.size(),
            [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    double accumulate_naive_vector(const std::vector<double>& inA)
    {
        return accumulate_Naive_Impl(inA.data(), inA.size());
    }

    namespace aligned {

        double accumulate_AVX_alvector(const alvector<double>& inA)
        {
            return accumulate_AVX_Impl(
                inA.data(), inA.size(),
                [](auto* inArray) { return _mm256_load_pd(inArray); });
        }

        double accumulate_naive_alvector(const alvector<double>& inA)
        {
            return accumulate_Naive_Impl(inA.data(), inA.size());
        }

    } // aligned
} // pml