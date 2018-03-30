#ifndef MATH_NUMERIC_SIMD_H
#define MATH_NUMERIC_SIMD_H

#pragma once

#include <PML/Core/NamespaceMacros.h>
#include <PML/Core/aligned_vector.h>

#include <intrin.h>

PML_MATH_NS_BEGIN

namespace simd {

    namespace detail {

        double inner_prod_aligned_AVX(
            const double* inA,
            const double* inB,
            std::size_t inSize);
    }

    inline double inner_prod_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        std::size_t inSize)
    {
        return detail::inner_prod_aligned_AVX(inA.get(), inB.get(), inSize);
    }

    inline double inner_prod_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB)
    {
        return detail::inner_prod_aligned_AVX(inA.data(), inB.data(), inA.size());
    }

    double inner_prod_AVX(
        const double* inA,
        const double* inB,
        std::size_t inSize);

    inline double inner_prod_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB)
    {
        return inner_prod_AVX(inA.data(), inB.data(), inA.size());
    }
}

PML_MATH_NS_END

#endif