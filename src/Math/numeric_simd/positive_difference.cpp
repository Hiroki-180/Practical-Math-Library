#include <PML/Core/cross_intrin.h>
#include <PML/Math/numeric_simd/positive_difference.h>
#include <cassert>
#include <algorithm>

namespace pml {

    namespace {

        template<typename T>
        void positive_difference_AVX_Impl(
            const double* inA,
            const double* inB,
            double* outC,
            std::size_t inSize,
            T inLoader)
        {
            const __m256d lZero256 = _mm256_setzero_pd();

            const std::size_t lUnrollEnd = (inSize - (inSize & 7));
            for (std::size_t i = 0; i < lUnrollEnd; i += 8)
            {
                __m256d lA256 = inLoader(&inA[i]);
                __m256d lB256 = inLoader(&inB[i]);
                __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                _mm256_storeu_pd(&outC[i], lPD256);

                lA256 = inLoader(&inA[i + 4]);
                lB256 = inLoader(&inB[i + 4]);
                lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                _mm256_storeu_pd(&outC[i + 4], lPD256);
            }

            const std::size_t l256End = (inSize - (inSize & 3));
            if (l256End != lUnrollEnd)
            {
                const __m256d lA256 = inLoader(&inA[lUnrollEnd]);
                const __m256d lB256 = inLoader(&inB[lUnrollEnd]);
                const __m256d lPD256 = _mm256_max_pd(lZero256, _mm256_sub_pd(lA256, lB256));
                _mm256_storeu_pd(&outC[lUnrollEnd], lPD256);
            }

            for (std::size_t i = l256End; i < inSize; ++i)
            {
                outC[i] = std::max(inA[i] - inB[i], 0.0);
            }
        }

        void positive_difference_naive_Impl(
            const double* inA,
            const double* inB,
            double* outC,
            std::size_t inSize)
        {
            for (std::size_t i = 0; i < inSize; ++i)
            {
                outC[i] = std::max(inA[i] - inB[i], 0.0);
            }
        }

    } // unnamed

    void positive_difference_AVX_array(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize)
    {
        return positive_difference_AVX_Impl(
                inA, inB, outC, inSize,
                [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    void positive_difference_naive_array(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize)
    {
        return positive_difference_naive_Impl(inA, inB, outC, inSize);
    }

    void positive_difference_AVX_vector(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC)
    {
        assert(inA.size() == inB.size());

        outC.resize(inA.size());
        outC.shrink_to_fit();

        return positive_difference_AVX_Impl(
                inA.data(), inB.data(), outC.data(), inA.size(),
                [](auto* inArray) { return _mm256_loadu_pd(inArray); });
    }

    void positive_difference_naive_vector(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC)
    {
        assert(inA.size() == inB.size());

        outC.resize(inA.size());
        outC.shrink_to_fit();
        
        return positive_difference_naive_Impl(inA.data(), inB.data(), outC.data(), inA.size());
    }

    namespace aligned {

        void positive_difference_AVX_alvector(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC)
        {
            assert(inA.size() == inB.size());

            outC.resize(inA.size());
            outC.shrink_to_fit();

            return positive_difference_AVX_Impl(
                    inA.data(), inB.data(), outC.data(), inA.size(),
                    [](auto* inArray) { return _mm256_load_pd(inArray); });
        }

        void positive_difference_naive_alvector(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC)
        {
            assert(inA.size() == inB.size());

            outC.resize(inA.size());
            outC.shrink_to_fit();
            
            return positive_difference_naive_Impl(inA.data(), inB.data(), outC.data(), inA.size());
        }

    } // aligned
} // pml