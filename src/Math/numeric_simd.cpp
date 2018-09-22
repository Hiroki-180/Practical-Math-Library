#include <PML/Core/CPUDispatcher.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Math/numeric_simd/accumulate.h>
#include <PML/Math/numeric_simd/inner_product.h>
#include <PML/Math/numeric_simd/positive_difference.h>
#include <PML/Math/numeric_simd/adjacent_divide.h>
#include <numeric>

namespace pml {

    double accumulate_SIMD(
        const double* inA,
        std::size_t inSize)
    {
        if (CPUDispatcher::isAVX())
        {
            return accumulate_AVX(inA, inSize);
        }

        return std::accumulate(inA, inA + inSize, 0.0);
    }

    double inner_product_SIMD(
        const double* inA,
        const double* inB,
        std::size_t inSize)
    {
        if (CPUDispatcher::isAVX())
        {
            return inner_product_AVX(inA, inB, inSize);
        }

        return std::inner_product(inA, inA + inSize, inB, 0.0);
    }

    void positive_difference_SIMD(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize)
    {
        if (CPUDispatcher::isAVX())
        {
            return positive_difference_AVX(inA, inB, outC, inSize);
        }

        for (std::size_t i = 0; i < inSize; ++i)
        {
            outC[i] = std::max(inA[i] - inB[i], 0.0);
        }
    }

    double accumulate_SIMD(const std::vector<double>& inA)
    {
        if (CPUDispatcher::isAVX())
        {
            return accumulate_AVX(inA);
        }

        return std::accumulate(inA.cbegin(), inA.cend(), 0.0);
    }

    double inner_product_SIMD(
        const std::vector<double>& inA,
        const std::vector<double>& inB)
    {
        if (CPUDispatcher::isAVX())
        {
            return inner_product_AVX(inA, inB);
        }

        return std::inner_product(inA.cbegin(), inA.cend(), inB.cbegin(), 0.0);
    }

    void positive_difference_SIMD(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC)
    {
        if (CPUDispatcher::isAVX())
        {
            positive_difference_AVX(inA, inB, outC);
            return;
        }

        outC.resize(inA.size());
        for (std::size_t i = 0; i < inA.size(); ++i)
        {
            outC[i] = std::max(inA[i] - inB[i], 0.0);
        }
        return;
    }

    namespace aligned {

        double accumulate_SIMD(const alvector<double>& inA)
        {
            if (CPUDispatcher::isAVX())
            {
                return aligned::accumulate_AVX(inA);
            }

            return std::accumulate(inA.cbegin(), inA.cend(), 0.0);
        }

        double inner_product_SIMD(
            const alvector<double>& inA,
            const alvector<double>& inB)
        {
            if (CPUDispatcher::isAVX())
            {
                return aligned::inner_product_AVX(inA, inB);
            }

            return std::inner_product(inA.cbegin(), inA.cend(), inB.cbegin(), 0.0);
        }

        void positive_difference_SIMD(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC)
        {
            if (CPUDispatcher::isAVX())
            {
                aligned::positive_difference_AVX(inA, inB, outC);
                return;
            }

            outC.resize(inA.size());
            for (std::size_t i = 0; i < inA.size(); ++i)
            {
                outC[i] = std::max(inA[i] - inB[i], 0.0);
            }
            return;
        }

    } //aligned
} //pml