#include <PML/Math/numeric_simd.h>

#include <intrin.h>
#include <algorithm>

namespace pml {

    namespace math {
        namespace numeric_simd {

            double accumulate_aligned_AVX(
                const double* inA,
                std::size_t inSize)
            {
                __m256d lSum256 = _mm256_setzero_pd();

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 0;i < lUnrollEnd;i += 8)
                {
                    const __m256d lF256 = _mm256_load_pd(&inA[i]);
                    const __m256d lB256 = _mm256_load_pd(&inA[i+4]);

                    lSum256 = _mm256_add_pd(lSum256, _mm256_add_pd(lF256, lB256));
                }

                const std::size_t l256End = (inSize - (inSize & 3));
                if (l256End != lUnrollEnd)
                {
                    lSum256 = _mm256_add_pd(lSum256, _mm256_load_pd(&inA[lUnrollEnd]));
                }

                const __m128d hiDual = _mm256_extractf128_pd(lSum256, 1);
                const __m128d loDual = _mm256_castpd256_pd128(lSum256);
                const __m128d lSum128 = _mm_add_pd(loDual, hiDual);

                PML_STATIC_ALLIGN(16) double lPartialSum[2] = { 0 };
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

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 0;i < lUnrollEnd;i += 8)
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

                for (std::size_t i = l256End;i < inSize; ++i)
                {
                    lSum += inA[i] * inB[i];
                }

                return lSum;
            }

            void adjacent_divide_aligned_AVX(
                const double* inA,
                double* outB,
                std::size_t inSize)
            {
                if (inSize == 0)
                {
                    return;
                }

                outB[inSize - 1] = inA[inSize - 1];

                if (inSize == 1)
                {
                    return;
                }

                outB[0] = inA[0] / inA[1];

                inSize -= 1;

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 1;i < lUnrollEnd;i += 8)
                {
                    __m256d lNum256  = _mm256_load_pd(&inA[i - 1]);
                    __m256d lDen256  = _mm256_load_pd(&inA[i    ]);
                    __m256d lDiv256 = _mm256_div_pd(lNum256, lDen256);
                    _mm256_store_pd(&outB[i - 1], lDiv256);

                    lNum256 = _mm256_load_pd(&inA[i + 3]);
                    lDen256 = _mm256_load_pd(&inA[i + 4]);
                    lDiv256 = _mm256_div_pd(lNum256, lDen256);
                    _mm256_store_pd(&outB[i + 3], lDiv256);
                }

                const std::size_t l256End = (inSize - (inSize & 3));
                if (l256End != lUnrollEnd)
                {
                    const __m256d lNum256  = _mm256_load_pd(&inA[lUnrollEnd - 1]);
                    const __m256d lDen256  = _mm256_load_pd(&inA[lUnrollEnd    ]);
                    const __m256d lDiv256 = _mm256_div_pd(lNum256, lDen256);
                    _mm256_store_pd(&outB[lUnrollEnd - 1], lDiv256);
                }

                for (std::size_t i = l256End;i < inSize; ++i)
                {
                    outB[i] = inA[i] / inA[i + 1];
                }

                return;
            }

            void positive_difference_aligned_AVX(
                const double* inA,
                const double* inB,
                double* outC,
                std::size_t inSize)
            {
                const __m256d lZero256 = _mm256_setzero_pd();

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 0;i < lUnrollEnd;i += 8)
                {
                    __m256d lA256  = _mm256_load_pd(&inA[i]);
                    __m256d lB256  = _mm256_load_pd(&inB[i]);
                    __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                    _mm256_store_pd(&outC[i], lPD256);

                    lA256  = _mm256_load_pd(&inA[i + 4]);
                    lB256  = _mm256_load_pd(&inB[i + 4]);
                    lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                    _mm256_store_pd(&outC[i + 4], lPD256);
                }

                const std::size_t l256End = (inSize - (inSize & 3));
                if (l256End != lUnrollEnd)
                {
                    const __m256d lA256  = _mm256_load_pd(&inA[lUnrollEnd]);
                    const __m256d lB256  = _mm256_load_pd(&inB[lUnrollEnd]);
                    const __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                    _mm256_store_pd(&outC[lUnrollEnd], lPD256);
                }

                for (std::size_t i = l256End;i < inSize; ++i)
                {
                    outC[i] = std::max(inA[i] - inB[i], 0.0);
                }

                return;
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

    void adjacent_divide_AVX(
        const double* inA,
        double* outB,
        std::size_t inSize)
    {
        if (inSize == 0)
        {
            return;
        }

        outB[inSize - 1] = inA[inSize - 1];

        if (inSize == 1)
        {
            return;
        }

        outB[0] = inA[0] / inA[1];

        inSize -= 1;

        const std::size_t lUnrollEnd = (inSize - (inSize & 7));
        for (std::size_t i = 1;i < lUnrollEnd;i += 8)
        {
            __m256d lNum256  = _mm256_loadu_pd(&inA[i - 1]);
            __m256d lDen256  = _mm256_loadu_pd(&inA[i    ]);
            __m256d lDiv256 = _mm256_div_pd(lNum256, lDen256);
            _mm256_store_pd(&outB[i - 1], lDiv256);

            lNum256  = _mm256_loadu_pd(&inA[i + 3]);
            lDen256  = _mm256_loadu_pd(&inA[i + 4]);
            lDiv256 = _mm256_div_pd(lNum256, lDen256);
            _mm256_store_pd(&outB[i + 3], lDiv256);
        }

        const std::size_t l256End = (inSize - (inSize & 3));
        if (l256End != lUnrollEnd)
        {
            const __m256d lNum256  = _mm256_loadu_pd(&inA[lUnrollEnd - 1]);
            const __m256d lDen256  = _mm256_loadu_pd(&inA[lUnrollEnd    ]);
            const __m256d lDeiv256 = _mm256_div_pd(lNum256, lDen256);
            _mm256_store_pd(&outB[lUnrollEnd - 1], lDeiv256);
        }

        for (std::size_t i = l256End;i < inSize; ++i)
        {
            outB[i] = inA[i] / inA[i + 1];
        }

        return;
    }

    void adjacent_divide_AVX(
        const std::vector<double>& inA,
        std::vector<double>& outB)
    {
        outB.resize(inA.size());
        adjacent_divide_AVX(inA.data(), outB.data(), inA.size());

        return;
    }

    void adjacent_divide_aligned_AVX(
        const aligned_array<double>& inA,
        aligned_array<double>& outB,
        std::size_t inSize)
    {
        math::numeric_simd::adjacent_divide_aligned_AVX(inA.get(), outB.get(), inSize);

        return;
    }

    void adjacent_divide_aligned_AVX(
        const align32_vector<double>& inA,
        align32_vector<double>& outB)
    {
        outB.resize(inA.size());
        math::numeric_simd::adjacent_divide_aligned_AVX(inA.data(), outB.data(), inA.size());

        return;
    }

    void positive_difference_AVX(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize)
    {
        const __m256d lZero256 = _mm256_setzero_pd();

        const std::size_t lUnrollEnd = (inSize - (inSize & 7));
        for (std::size_t i = 0;i < lUnrollEnd;i += 8)
        {
            __m256d lA256  = _mm256_loadu_pd(&inA[i]);
            __m256d lB256  = _mm256_loadu_pd(&inB[i]);
            __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
            _mm256_storeu_pd(&outC[i], lPD256);

            lA256  = _mm256_loadu_pd(&inA[i + 4]);
            lB256  = _mm256_loadu_pd(&inB[i + 4]);
            lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
            _mm256_storeu_pd(&outC[i + 4], lPD256);
        }

        const std::size_t l256End = (inSize - (inSize & 3));
        if (l256End != lUnrollEnd)
        {
            const __m256d lA256  = _mm256_loadu_pd(&inA[lUnrollEnd]);
            const __m256d lB256  = _mm256_loadu_pd(&inB[lUnrollEnd]);
            const __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
            _mm256_storeu_pd(&outC[lUnrollEnd], lPD256);
        }

        for (std::size_t i = l256End; i < inSize; ++i)
        {
            outC[i] = std::max(inA[i] - inB[i], 0.0);
        }

        return;
    }

    void positive_difference_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC)
    {
        outC.resize(inA.size());
        positive_difference_AVX(inA.data(), inB.data(), outC.data(), inA.size());

        return;
    }

    void positive_difference_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        aligned_array<double>& outC,
        std::size_t inSize)
    {
        math::numeric_simd::positive_difference_aligned_AVX(inA.get(), inB.get(), outC.get(), inSize);

        return;
    }

    void positive_difference_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB,
        align32_vector<double>& outC)
    {
        outC.resize(inA.size());
        math::numeric_simd::positive_difference_aligned_AVX(inA.data(), inB.data(), outC.data(), inA.size());

        return;
    }

} // pml