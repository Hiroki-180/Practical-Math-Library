#ifndef MATH_NUMERIC_SIMD_H
#define MATH_NUMERIC_SIMD_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml{

    double accumulate_AVX(
        const double* inA,
        std::size_t inSize);

    double accumulate_AVX(const std::vector<double>& inA);

    double accumulate_aligned_AVX(
        const aligned_array<double>& inA,
        std::size_t inSize);

    double accumulate_aligned_AVX(const align32_vector<double>& inA);

    double inner_prod_AVX(
        const double* inA,
        const double* inB,
        std::size_t inSize);

    double inner_prod_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB);

    double inner_prod_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        std::size_t inSize);

    double inner_prod_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB);

} // pml

#endif