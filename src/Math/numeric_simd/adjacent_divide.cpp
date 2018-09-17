#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/adjacent_divide.h>

namespace pml {

    namespace {

        template<typename T>
        void adjacent_divide_AVX_Impl(
            const double* inA,
            double inShift,
            double* outB,
            std::size_t inSize,
            T inLoader)
        {
            const __m256d lShift = _mm256_set1_pd(inShift);
            const std::size_t l256End = (inSize - (inSize & 3));
            __m256d lA256 = inLoader(&inA[0]);

            if (l256End != 0)
            {
                const __m256d lB256 = aligned::_mm256_rotate_left_pd(lA256, -1);
                const __m256d lDiv256 = _mm256_add_pd(_mm256_div_pd(lB256, lA256), lShift);
                _mm256_store_pd(&outB[0], lDiv256);

                outB[0] = inA[0];
            }

            const __m256d lMask256 = _mm256_set_pd(-1, 0, 0, 0);
            for (std::size_t i = 4; i < l256End; i += 4)
            {
                // A: inA[i-4], inA[i-3], inA[i-2], inA[i-1]

                // B: inA[i], inA[i+1], inA[i+2], inA[i+3]
                __m256d lB256 = inLoader(&inA[i]);

                // C: inA[i], inA[i+1], inA[i+2], inA[i-1]
                __m256d lC256 = _mm256_blendv_pd(lB256, lA256, lMask256);

                // D: inA[i-1], inA[i], inA[i+1], inA[i+2]
                const __m256d lD256 = aligned::_mm256_rotate_left_pd(lC256, -1);

                // Div: inA[i-1]/inA[i], inA[i]/inA[i+1], inA[i+1]/inA[i+2], inA[i+2]/inA[i+3]
                const __m256d lDiv256 = _mm256_add_pd(_mm256_div_pd(lD256, lB256), lShift);
                _mm256_store_pd(&outB[i], lDiv256);

                std::swap(lA256, lB256);
            }

            for (std::size_t i = l256End; i < inSize; ++i)
            {
                outB[i] = inA[i - 1] / inA[i] + inShift;
            }
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