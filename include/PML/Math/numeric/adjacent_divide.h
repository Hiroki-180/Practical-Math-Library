#ifndef MATH_NUMERIC_ADJACENT_DIVIDE_H
#define MATH_NUMERIC_ADJACENT_DIVIDE_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml{

    /**
    * @fn adjacent_divide_AVX(const double* inA, double* outB, std::size_t inSize)
    *
    * @brief
    * Dividing elements by the adjacent elements.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the input array.
    *
    * @param[out] outB
    * Resulted array, (inA[0] / inA[1], inA[1] / inA[2], ..., inA[inSize-2] / inA[inSize-1], inA[inSize-1]).
    *
    * @param[in] inSize
    * Size of the array inA and outB.
    */
    void adjacent_divide_AVX(
        const double* inA,
        double* outB,
        std::size_t inSize);

    /**
    * @fn adjacent_divide_AVX(const std::vector<double>& inA, std::vector<double>& outB)
    *
    * @brief
    * Dividing elements by the adjacent elements.
    *
    * @param[in] inA
    * Input array as vector.
    *
    * @param[out] outB
    * Resulted array as vector, (inA[0] / inA[1], inA[1] / inA[2], ..., inA[inSize-2] / inB[inSize-1], inA[inSize-1]).
    */
    void adjacent_divide_AVX(
        const std::vector<double>& inA,
        std::vector<double>& outB);

    /**
    * @fn adjacent_divide_aligned_AVX(const aligned_array<double>& inA, aligned_array<double>& outB, std::size_t inSize)
    *
    * @brief
    * Dividing elements by the adjacent elements.
    *
    * @param[in] inA
    * 32-byte aligned input array.
    *
    * @param[out] outB
    * 32-byte aligned resulted array, (inA[0] / inA[1], inA[1] / inA[2], ..., inA[inSize-2] / inA[inSize-1], inA[inSize-1]).
    *
    * @param[in] inSize
    * Size of the array inA and outB.
    */
    void adjacent_divide_aligned_AVX(
        const aligned_array<double>& inA,
        aligned_array<double>& outB,
        std::size_t inSize);

    /**
    * @fn adjacent_divide_aligned_AVX(const align32_vector<double>& inA, align32_vector<double>& outB)
    *
    * @brief
    * Dividing elements by the adjacent elements.
    *
    * @param[in] inA
    * 32-byte aligned input array as vector.
    *
    * @param[out] outB
    * 32-byte aligned resulted array as vector, (inA[0] / inA[1], inA[1] / inA[2], ..., inA[inSize-2] / inA[inSize-1], inA[inSize-1]).
    */
    void adjacent_divide_aligned_AVX(
        const align32_vector<double>& inA,
        align32_vector<double>& outB);

} // pml

#endif