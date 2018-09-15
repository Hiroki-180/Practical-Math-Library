#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/positive_difference.h>
#include <cassert>
#include <algorithm>

namespace pml {

    void positive_difference_AVX(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize)
    {
        const __m256d lZero256 = _mm256_setzero_pd();

        const std::size_t lUnrollEnd = (inSize - (inSize & 7));
        for (std::size_t i = 0; i < lUnrollEnd; i += 8)
        {
            __m256d lA256 = _mm256_loadu_pd(&inA[i]);
            __m256d lB256 = _mm256_loadu_pd(&inB[i]);
            __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
            _mm256_storeu_pd(&outC[i], lPD256);

            lA256 = _mm256_loadu_pd(&inA[i + 4]);
            lB256 = _mm256_loadu_pd(&inB[i + 4]);
            lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
            _mm256_storeu_pd(&outC[i + 4], lPD256);
        }

        const std::size_t l256End = (inSize - (inSize & 3));
        if (l256End != lUnrollEnd)
        {
            const __m256d lA256 = _mm256_loadu_pd(&inA[lUnrollEnd]);
            const __m256d lB256 = _mm256_loadu_pd(&inB[lUnrollEnd]);
            const __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
            _mm256_storeu_pd(&outC[lUnrollEnd], lPD256);
        }

        for (std::size_t i = l256End; i < inSize; ++i)
        {
            outC[i] = std::max(inA[i] - inB[i], 0.0);
        }
    }

    void positive_difference_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC)
    {
        outC.resize(inA.size());
        positive_difference_AVX(inA.data(), inB.data(), outC.data(), inA.size());
    }


    namespace aligned {
        namespace detail {

            void positive_difference_AVX(
                const double* inA,
                const double* inB,
                double* outC,
                std::size_t inSize)
            {
                assert((uintptr_t)(&(inA[0])) % 32 == 0U);

                const __m256d lZero256 = _mm256_setzero_pd();

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 0; i < lUnrollEnd; i += 8)
                {
                    __m256d lA256 = _mm256_load_pd(&inA[i]);
                    __m256d lB256 = _mm256_load_pd(&inB[i]);
                    __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                    _mm256_store_pd(&outC[i], lPD256);

                    lA256 = _mm256_load_pd(&inA[i + 4]);
                    lB256 = _mm256_load_pd(&inB[i + 4]);
                    lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                    _mm256_store_pd(&outC[i + 4], lPD256);
                }

                const std::size_t l256End = (inSize - (inSize & 3));
                if (l256End != lUnrollEnd)
                {
                    const __m256d lA256 = _mm256_load_pd(&inA[lUnrollEnd]);
                    const __m256d lB256 = _mm256_load_pd(&inB[lUnrollEnd]);
                    const __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                    _mm256_store_pd(&outC[lUnrollEnd], lPD256);
                }

                for (std::size_t i = l256End; i < inSize; ++i)
                {
                    outC[i] = std::max(inA[i] - inB[i], 0.0);
                }
            }
        }

        void positive_difference_AVX(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC)
        {
            outC.resize(inA.size());
            detail::positive_difference_AVX(inA.data(), inB.data(), outC.data(), inA.size());
        }

    } // aligned
} // pml