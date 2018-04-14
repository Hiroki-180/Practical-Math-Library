#ifndef MATH_NUMERIC_POSITIVE_DIFFERENCE_H
#define MATH_NUMERIC_POSITIVE_DIFFERENCE_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml{

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
    * 1st array as std::vector.
    *
    * @param[out] inB
    * 2nd array as std::vector.
    *
    * @param[in] outC
    * Resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
    */
    void positive_difference_AVX(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC);


    namespace aligned {

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
        void positive_difference_AVX(
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
        void positive_difference_AVX(
            const align32_vector<double>& inA,
            const align32_vector<double>& inB,
            align32_vector<double>& outC);

    } // aligned
} // pml

#endif