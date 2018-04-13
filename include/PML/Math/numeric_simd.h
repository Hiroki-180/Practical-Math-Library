#ifndef MATH_NUMERIC_SIMD_H
#define MATH_NUMERIC_SIMD_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml{

    /**
    * @fn accumulate_AVX(const double* inA, std::size_t inSize)
    *
    * @brief
    * Accelerated version of std::accumulate by AVX.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the array.
    *
    * @param[in] inSize
    * Size of the array inA.
    *
    * @return
    * Sum of the all elements of the input array, inA[0] + inA[1] + ... + inA[inSize-1].
    */
    double accumulate_AVX(
        const double* inA,
        std::size_t inSize);

    /**
    * @fn accumulate_AVX(const std::vector<double>& inA);
    *
    * @brief
    * Accelerated version of std::accumulate by AVX.
    *
    * @param[in] inA
    * Array as std::vector.
    *
    * @return
    * Sum of the all elements of the input array, inA[0] + inA[1] + ... + inA[inA.size()-1].
    */
    double accumulate_AVX(const std::vector<double>& inA);

    /**
    * @fn accumulate_aligned_AVX(const aligned_array<double>& inA, std::size_t inSize)
    *
    * @brief
    * Accelerated version of std::accumulate by AVX with memory aligned data.
    *
    * @param[in] inA
    * 32-byte aligned array.
    *
    * @param[in] inSize
    * Size of the array inA.
    *
    * @return
    * Sum of the all elements of the input array, inA[0] + inA[1] + ... + inA[inSize-1].
    */
    double accumulate_aligned_AVX(
        const aligned_array<double>& inA,
        std::size_t inSize);

    /**
    * @fn accumulate_aligned_AVX(const align32_vector<double>& inA)
    *
    * @brief
    * Accelerated version of std::accumulate by AVX with memory aligned data.
    *
    * @param[in] inA
    * 32-byte aligned vector.
    *
    * @return
    * Sum of the all elements of the input array, inA[0] + inA[1] + ... + inA[inA.size()-1].
    */
    double accumulate_aligned_AVX(const align32_vector<double>& inA);

    /**
    * @fn inner_prod_AVX(const double* inA, const double* inB, std::size_t inSize)
    *
    * @brief
    * Accelerated version of std::inner_prod by AVX.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the 1st array.
    *
    * @param[in] inB
    * Pointer to the address of the first element of the 2nd array.
    *
    * @param[in] inSize
    * Size of the array inA and inB.
    *
    * @return
    * Inner product of the input arrays, inA[0]*inB[0] + inA[1]*inB[1] + ... + inA[inSize-1]*inB[inSize-1].
    */
    double inner_prod_AVX(
        const double* inA,
        const double* inB,
        std::size_t inSize);

    /**
    * @fn inner_prod_AVX(const std::vector<double>& inA, const std::vector<double>& inB)
    *
    * @brief
    * Accelerated version of std::inner_prod by AVX.
    *
    * @param[in] inA
    * 1st array as std::vector.
    *
    * @param[in] inB
    * 2nd array as std::vector.
    *
    * @return
    * Inner product of the input arrays, inA[0]*inB[0] + inA[1]*inB[1] + ... + inA[inA.size()-1]*inB[inB.size()-1].
    */
    double inner_prod_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB);

    /**
    * @fn inner_prod_aligned_AVX(const aligned_array<double>& inA, const aligned_array<double>& inB, std::size_t inSize)
    *
    * @brief
    * Accelerated version of std::inner_prod by AVX with memory aligned data.
    *
    * @param[in] inA
    * 32-byte aligned 1st array.
    *
    * @param[in] inB
    * 32-byte aligned 2nd array.
    *
    * @param[in] inSize
    * Size of the array inA and inB.
    *
    * @return
    * Inner product of the input arrays, inA[0]*inB[0] + inA[1]*inB[1] + ... + inA[inSize-1]*inB[inSize-1].
    */
    double inner_prod_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        std::size_t inSize);

    /**
    * @fn inner_prod_aligned_AVX(const align32_vector<double>& inA, const align32_vector<double>& inB)
    *
    * @brief
    * Accelerated version of std::inner_prod by AVX with memory aligned data.
    *
    * @param[in] inA
    * 32-byte aligned 1st vector.
    *
    * @param[in] inB
    * 32-byte aligned 2nd vector.
    *
    * @return
    * Inner product of the input arrays, inA[0]*inB[0] + inA[1]*inB[1] + ... + inA[inA.size()-1]*inB[inB.size()-1].
    */
    double inner_prod_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB);

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

    /**
    * @fn positive_difference_AVX(const double* inA, const double* inB, double* outC, std::size_t inSize)
    *
    * @brief
    * Instruction of subtraction of two arrays and taking plus part.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the 1st array.
    *
    * @param[in] inB
    * Pointer to the address of the first element of the 2nd array.
    *
    * @param[out] outC
    * Resulted array, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inSize-1] - inB[inSize-1], 0)).
    *
    * @param[in] inSize
    * Size of the array inA, inB and outC.
    */
    void positive_difference_AVX(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize);

    /**
    * @fn positive_difference_AVX(const std::vector<double>& inA, const std::vector<double>& inB, std::vector<double>& outC)
    *
    * @brief
    * Instruction of subtraction of two arrays and taking plus part.
    *
    * @param[in] inA
    * 1st array as vector.
    *
    * @param[out] inB
    * 2nd array as vector.
    *
    * @param[in] outC
    * Resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
    */
    void positive_difference_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC);

    /**
    * @fn positive_difference_aligned_AVX(const aligned_array<double>& inA, const aligned_array<double>& inB, aligned_array<double>& outC, std::size_t inSize)
    *
    * @brief
    * Instruction of subtraction of two arrays and taking plus part.
    *
    * @param[in] inA
    * 32-byte aligned 1st array.
    *
    * @param[in] inB
    * 32-byte aligned 2nd array.
    *
    * @param[out] outC
    * 32-byte aligned resulted array, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inSize-1] - inB[inSize-1], 0)).
    *
    * @param[in] inSize
    * Size of the array inA, inB and outC.
    */
    void positive_difference_aligned_AVX(
        const aligned_array<double>& inA,
        const aligned_array<double>& inB,
        aligned_array<double>& outC,
        std::size_t inSize);

    /**
    * @fn positive_difference_aligned_AVX(const align32_vector<double>& inA, const align32_vector<double>& inB, align32_vector<double>& outC)
    *
    * @brief
    * Instruction of subtraction of two arrays and taking plus part.
    *
    * @param[in] inA
    * 32-byte aligned 1st array as vector.
    *
    * @param[in] inB
    * 32-byte aligned 2nd array as vector.
    *
    * @param[out] outC
    * 32-byte aligned resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
    */
    void positive_difference_aligned_AVX(
        const align32_vector<double>& inA,
        const align32_vector<double>& inB,
        align32_vector<double>& outC);

} // pml

#endif