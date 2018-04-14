#include <PML/Math/numeric/adjacent_divide.h>

#include <intrin.h>
#include <algorithm>

namespace pml {

    void adjacent_divide_AVX(
        const double* inA,
        double inShift,
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

        inSize -= 1;

        const std::size_t lUnrollEnd = (inSize - (inSize & 7));
        for (std::size_t i = 1;i < lUnrollEnd;i += 8)
        {
            __m256d lNum256 = _mm256_loadu_pd(&inA[i - 1]);
            __m256d lDen256 = _mm256_loadu_pd(&inA[i]);
            __m256d lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
            _mm256_store_pd(&outB[i - 1], lDiv256);

            lNum256 = _mm256_loadu_pd(&inA[i + 3]);
            lDen256 = _mm256_loadu_pd(&inA[i + 4]);
            lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
            _mm256_store_pd(&outB[i + 3], lDiv256);
        }

        const std::size_t l256End = (inSize - (inSize & 3));
        if (l256End != lUnrollEnd)
        {
            const __m256d lNum256  = _mm256_loadu_pd(&inA[lUnrollEnd - 1]);
            const __m256d lDen256  = _mm256_loadu_pd(&inA[lUnrollEnd]);
            const __m256d lDeiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
            _mm256_store_pd(&outB[lUnrollEnd - 1], lDeiv256);
        }

        for (std::size_t i = l256End;i < inSize; ++i)
        {
            outB[i] = inA[i] / inA[i + 1] + inShift;
        }
    }

    void adjacent_divide_AVX(
        const std::vector<double>& inA,
        double inShift,
        std::vector<double>& outB)
    {
        outB.resize(inA.size());
        adjacent_divide_AVX(inA.data(), inShift, outB.data(), inA.size());
    }


    namespace aligned {

        namespace detail {

            void adjacent_divide_AVX(
                const double* inA,
                double inShift,
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

                inSize -= 1;

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 1;i < lUnrollEnd;i += 8)
                {
                    __m256d lNum256 = _mm256_load_pd(&inA[i - 1]);
                    __m256d lDen256 = _mm256_load_pd(&inA[i]);
                    __m256d lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
                    _mm256_store_pd(&outB[i - 1], lDiv256);

                    lNum256 = _mm256_load_pd(&inA[i + 3]);
                    lDen256 = _mm256_load_pd(&inA[i + 4]);
                    lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
                    _mm256_store_pd(&outB[i + 3], lDiv256);
                }

                const std::size_t l256End = (inSize - (inSize & 3));
                if (l256End != lUnrollEnd)
                {
                    const __m256d lNum256 = _mm256_load_pd(&inA[lUnrollEnd - 1]);
                    const __m256d lDen256 = _mm256_load_pd(&inA[lUnrollEnd]);
                    const __m256d lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
                    _mm256_store_pd(&outB[lUnrollEnd - 1], lDiv256);
                }

                for (std::size_t i = l256End;i < inSize; ++i)
                {
                    outB[i] = inA[i] / inA[i + 1] + inShift;
                }
            }
        }

        void adjacent_divide_AVX(
            const aligned_array<double>& inA,
            double inShift,
            aligned_array<double>& outB,
            std::size_t inSize)
        {
            detail::adjacent_divide_AVX(inA.get(), inShift, outB.get(), inSize);
        }

        void adjacent_divide_AVX(
            const align32_vector<double>& inA,
            double inShift,
            align32_vector<double>& outB)
        {
            outB.resize(inA.size());
            detail::adjacent_divide_AVX(inA.data(), inShift, outB.data(), inA.size());
        }

    } // aligned
} // pml