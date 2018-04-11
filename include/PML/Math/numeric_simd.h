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


    void positive_difference_AVX(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize);

    void positive_difference_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC);

    void positive_difference_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        aligned_array<double>& outC,
        std::size_t inSize);

    void positive_difference_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB,
        align32_vector<double>& outC,
        std::size_t inSize);

} // pml

#endif