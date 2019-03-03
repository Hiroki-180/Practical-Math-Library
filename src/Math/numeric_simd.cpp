#include <PML/Core/CPUDispatcher.h>
#include <PML/Math/numeric_simd.h>
#include <PML/Math/numeric_simd/positive_difference.h>
#include <numeric>
#include <functional>

namespace pml {

    namespace {

        // select optimal function for the runtime CPU
        template<typename F, typename... Args>
        auto optimizer(F inAVX512, F inAVX, F inSSE, F inNaive, Args&... inArgs)
        {
            if (CPUDispatcher::isAVX512F()){
                return inAVX512(inArgs...);
            }
            else if (CPUDispatcher::isAVX()){
                return inAVX(inArgs...);
            }
            else if (CPUDispatcher::isSSE()){
                return inSSE(inArgs...);
            }

            return inNaive(inArgs...);
        }

    } // unnamed

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
                &positive_difference_array,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            std::bind(
                &positive_difference_array,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            inA, inB, outC, inSize);
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
                &positive_difference_vector,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            std::bind(
                &positive_difference_vector,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
            inA, inB, outC);
    }

    namespace aligned {

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
                    &positive_difference_alvector,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                std::bind(
                    &positive_difference_alvector,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                inA, inB, outC);
        }

    } //aligned
} //pml