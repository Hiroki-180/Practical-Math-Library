#include <PML/Core/CPUDispatcher.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Math/numeric_simd/accumulate.h>
#include <PML/Math/numeric_simd/inner_product.h>
#include <PML/Math/numeric_simd/positive_difference.h>
#include <numeric>
#include <functional>

namespace pml {

    namespace {

        // select optimal function for the runtime CPU
        template<typename F, typename... Args>
        auto optimizer(F inAVX512, F inAVX, F inSSE, F inNaive, Args&... inArgs)
        {
            if (CPUDispatcher::isAVX512F())
            {
                return inAVX512(inArgs...);
            }
            else if (CPUDispatcher::isAVX())
            {
                return inAVX(inArgs...);
            }
            else if (CPUDispatcher::isSSE())
            {
                return inSSE(inArgs...);
            }

            return inNaive(inArgs...);
        }

    } // unnamed

    double accumulate_SIMD(
        const double* inA,
        std::size_t inSize)
    {
        return optimizer(
            std::bind(&accumulate_AVX_array,   std::placeholders::_1, std::placeholders::_2),
            std::bind(&accumulate_AVX_array,   std::placeholders::_1, std::placeholders::_2),
            std::bind(&accumulate_naive_array, std::placeholders::_1, std::placeholders::_2),
            std::bind(&accumulate_naive_array, std::placeholders::_1, std::placeholders::_2),
            inA, inSize);
    }

    double inner_product_SIMD(
        const double* inA,
        const double* inB,
        std::size_t inSize)
    {
        return optimizer(
            std::bind(&inner_product_AVX_array,   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&inner_product_AVX_array,   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&inner_product_naive_array, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(&inner_product_naive_array, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            inA, inB, inSize);
    }

    void positive_difference_SIMD(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize)
    {
        return optimizer(
            std::bind(
                &positive_difference_AVX_array,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            std::bind(
                &positive_difference_AVX_array,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            std::bind(
                &positive_difference_naive_array,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            std::bind(
                &positive_difference_naive_array,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            inA, inB, outC, inSize);
    }

    double accumulate_SIMD(const std::vector<double>& inA)
    {
        return optimizer(
            std::bind(&accumulate_AVX_vector,   std::placeholders::_1),
            std::bind(&accumulate_AVX_vector,   std::placeholders::_1),
            std::bind(&accumulate_naive_vector, std::placeholders::_1),
            std::bind(&accumulate_naive_vector, std::placeholders::_1),
            inA);
    }

    double inner_product_SIMD(
        const std::vector<double>& inA,
        const std::vector<double>& inB)
    {
        return optimizer(
            std::bind(&inner_product_AVX_vector,   std::placeholders::_1, std::placeholders::_2),
            std::bind(&inner_product_AVX_vector,   std::placeholders::_1, std::placeholders::_2),
            std::bind(&inner_product_naive_vector, std::placeholders::_1, std::placeholders::_2),
            std::bind(&inner_product_naive_vector, std::placeholders::_1, std::placeholders::_2),
            inA, inB);
    }

    void positive_difference_SIMD(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC)
    {
        return optimizer(
            std::bind(
                &positive_difference_AVX_vector,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(
                &positive_difference_AVX_vector,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(
                &positive_difference_naive_vector,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(
                &positive_difference_naive_vector,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            inA, inB, outC);
    }

    namespace aligned {

        double accumulate_SIMD(const alvector<double>& inA)
        {
            return optimizer(
                std::bind(&accumulate_AVX_alvector,   std::placeholders::_1),
                std::bind(&accumulate_AVX_alvector,   std::placeholders::_1),
                std::bind(&accumulate_naive_alvector, std::placeholders::_1),
                std::bind(&accumulate_naive_alvector, std::placeholders::_1),
                inA);
        }

        double inner_product_SIMD(
            const alvector<double>& inA,
            const alvector<double>& inB)
        {
            return optimizer(
                std::bind(&inner_product_AVX_alvector,   std::placeholders::_1, std::placeholders::_2),
                std::bind(&inner_product_AVX_alvector,   std::placeholders::_1, std::placeholders::_2),
                std::bind(&inner_product_naive_alvector, std::placeholders::_1, std::placeholders::_2),
                std::bind(&inner_product_naive_alvector, std::placeholders::_1, std::placeholders::_2),
                inA, inB);
        }

        void positive_difference_SIMD(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC)
        {
            return optimizer(
                std::bind(
                    &positive_difference_AVX_alvector,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(
                    &positive_difference_AVX_alvector,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(
                    &positive_difference_naive_alvector,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(
                    &positive_difference_naive_alvector,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                inA, inB, outC);
        }

    } //aligned
} //pml