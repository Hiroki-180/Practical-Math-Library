#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/inner_product.h>
#include <cassert>
#include <numeric>

namespace pml {

    namespace {

        template<typename T>
        double inner_product_AVX_Impl(
            const double* inA,
            const double* inB,
            std::size_t inSize,
            T inLoader)
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

            PML_STATIC_ALLIGN(16) double lPartialSum[2] = { 0 };
            _mm_store_pd(lPartialSum, lSum128);
            auto lSum = lPartialSum[0] + lPartialSum[1];

            for (std::size_t i = l256End; i < inSize; ++i){
                lSum += inA[i] * inB[i];
            }

            return lSum;
        }

    } // unnamed

    double inner_product_AVX_array(
        const double* inA,
        const double* inB,
        std::size_t inSize)
    {
        return inner_product_AVX_Impl(
            inA, inB, inSize,
            [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }
        
    double inner_product_array(
        const double* inA,
        const double* inB,
        std::size_t inSize)
    {
        return std::inner_product(inA, inA + inSize, inB, 0.0);
    }

    double inner_product_AVX_vector(
        const std::vector<double>& inA,
        const std::vector<double>& inB)
    {
        assert(inA.size() == inB.size());

        return inner_product_AVX_Impl(
            inA.data(), inB.data(), inA.size(),
            [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    double inner_product_vector(
        const std::vector<double>& inA,
        const std::vector<double>& inB)
    {
        assert(inA.size() == inB.size());

        return std::inner_product(inA.cbegin(), inA.cend(), inB.cend(), 0.0);
    }

    namespace aligned {

        double inner_product_AVX_alvector(
            const alvector<double>& inA,
            const alvector<double>& inB)
        {
            assert(inA.size() == inB.size());

            return inner_product_AVX_Impl(
                inA.data(), inB.data(), inA.size(),
                [](auto* inArray) { return _mm256_load_pd(inArray); });
        }

        double inner_product_alvector(
            const alvector<double>& inA,
            const alvector<double>& inB)
        {
            assert(inA.size() == inB.size());

            return std::inner_product(inA.cbegin(), inA.cend(), inB.cbegin(), 0.0);
        }

    } // aligned
} // pml