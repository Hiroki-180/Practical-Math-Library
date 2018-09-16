#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/adjacent_divide.h>

namespace pml {

    template<typename T>
    void adjacent_divide_AVX_Impl(
        const double* inA,
        double inShift,
        double* outB,
        std::size_t inSize,
        T inLoader)
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
        for (std::size_t i = 1; i < lUnrollEnd; i += 8)
        {
            __m256d lNum256 = inLoader(&inA[i - 1]);
            __m256d lDen256 = inLoader(&inA[i]);
            __m256d lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
            _mm256_store_pd(&outB[i - 1], lDiv256);

            lNum256 = inLoader(&inA[i + 3]);
            lDen256 = inLoader(&inA[i + 4]);
            lDiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
            _mm256_store_pd(&outB[i + 3], lDiv256);
        }

        const std::size_t l256End = (inSize - (inSize & 3));
        if (l256End != lUnrollEnd)
        {
            const __m256d lNum256 = inLoader(&inA[lUnrollEnd - 1]);
            const __m256d lDen256 = inLoader(&inA[lUnrollEnd]);
            const __m256d lDeiv256 = _mm256_add_pd(_mm256_div_pd(lNum256, lDen256), _mm256_set1_pd(inShift));
            _mm256_store_pd(&outB[lUnrollEnd - 1], lDeiv256);
        }

        for (std::size_t i = l256End; i < inSize; ++i)
        {
            outB[i] = inA[i] / inA[i + 1] + inShift;
        }
    }

    void adjacent_divide_AVX(
        const double* inA,
        double inShift,
        double* outB,
        std::size_t inSize)
    {
        return adjacent_divide_AVX_Impl(
            inA, inShift, outB, inSize,
            [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    void adjacent_divide_AVX(
        const std::vector<double>& inA,
        double inShift,
        std::vector<double>& outB)
    {
        outB.resize(inA.size());

        adjacent_divide_AVX_Impl(
            inA.data(), inShift, outB.data(), inA.size(),
            [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    namespace aligned {

        void adjacent_divide_AVX(
            const alvector<double>& inA,
            double inShift,
            alvector<double>& outB)
        {
            outB.resize(inA.size());

            adjacent_divide_AVX_Impl(
                inA.data(), inShift, outB.data(), inA.size(),
                [](auto* inArray) { return _mm256_load_pd(inArray); });
        }

    } // aligned
} // pml