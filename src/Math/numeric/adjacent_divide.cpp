#include <PML/Math/numeric/adjacent_divide.h>

#include <intrin.h>
#include <algorithm>

namespace pml {

    namespace math {
        namespace numeric_simd {

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

                inSize -= 1;

                const std::size_t lUnrollEnd = (inSize - (inSize & 7));
                for (std::size_t i = 1;i < lUnrollEnd;i += 8)
                {
                    __m256d lNum256 = _mm256_load_pd(&inA[i - 1]);
                    __m256d lDen256 = _mm256_load_pd(&inA[i]);
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
                    const __m256d lNum256 = _mm256_load_pd(&inA[lUnrollEnd - 1]);
                    const __m256d lDen256 = _mm256_load_pd(&inA[lUnrollEnd]);
                    const __m256d lDiv256 = _mm256_div_pd(lNum256, lDen256);
                    _mm256_store_pd(&outB[lUnrollEnd - 1], lDiv256);
                }

                for (std::size_t i = l256End;i < inSize; ++i)
                {
                    outB[i] = inA[i] / inA[i + 1];
                }

                return;
            }

        }
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

        inSize -= 1;

        const std::size_t lUnrollEnd = (inSize - (inSize & 7));
        for (std::size_t i = 1;i < lUnrollEnd;i += 8)
        {
            __m256d lNum256 = _mm256_loadu_pd(&inA[i - 1]);
            __m256d lDen256 = _mm256_loadu_pd(&inA[i]);
            __m256d lDiv256 = _mm256_div_pd(lNum256, lDen256);
            _mm256_store_pd(&outB[i - 1], lDiv256);

            lNum256 = _mm256_loadu_pd(&inA[i + 3]);
            lDen256 = _mm256_loadu_pd(&inA[i + 4]);
            lDiv256 = _mm256_div_pd(lNum256, lDen256);
            _mm256_store_pd(&outB[i + 3], lDiv256);
        }

        const std::size_t l256End = (inSize - (inSize & 3));
        if (l256End != lUnrollEnd)
        {
            const __m256d lNum256 = _mm256_loadu_pd(&inA[lUnrollEnd - 1]);
            const __m256d lDen256 = _mm256_loadu_pd(&inA[lUnrollEnd]);
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

} // pml