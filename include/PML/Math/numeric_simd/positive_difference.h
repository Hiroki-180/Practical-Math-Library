#ifndef MATH_NUMERIC_POSITIVE_DIFFERENCE_H
#define MATH_NUMERIC_POSITIVE_DIFFERENCE_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml {

    /**
    * @brief
    * Instruction of subtraction of two arrays and taking plus part using AVX instructions.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the 1st array.
    *
    * @param[in] inB
    * Pointer to the address of the first element of the 2nd array which should have same size with inA.
    *
    * @param[out] outC
    * Resulted array, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inSize-1] - inB[inSize-1], 0)).
    * The size is automatically adjusted to inA and inB in this function.
    *
    * @param[in] inSize
    * Size of the array inA, inB and outC.
    */
    void positive_difference_AVX_array(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize);

    /**
    * @brief
    * Instruction of subtraction of two arrays and taking plus part using no SIMD instructions.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the 1st array.
    *
    * @param[in] inB
    * Pointer to the address of the first element of the 2nd array which should have same size with inA.
    *
    * @param[out] outC
    * Resulted array, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inSize-1] - inB[inSize-1], 0)).
    * The size is automatically adjusted to inA and inB in this function.
    *
    * @param[in] inSize
    * Size of the array inA, inB and outC.
    */
    void positive_difference_array(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize);

    /**
    * @brief
    * Instruction of subtraction of two arrays and taking plus part using AVX instructions.
    *
    * @param[in] inA
    * 1st array as std::vector.
    *
    * @param[out] inB
    * 32-byte aligned 2nd array as vector which should have same size with inA.
    *
    * @param[in] outC
    * Resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
    * The size is automatically adjusted to inA and inB in this function.
    */
    void positive_difference_AVX_vector(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC);

    /**
    * @brief
    * Instruction of subtraction of two arrays and taking plus part using no SIMD instructions.
    *
    * @param[in] inA
    * 1st array as std::vector.
    *
    * @param[out] inB
    * 32-byte aligned 2nd array as vector which should have same size with inA.
    *
    * @param[in] outC
    * Resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
    * The size is automatically adjusted to inA and inB in this function.
    */
    void positive_difference_vector(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC);

    namespace aligned {

        /**
        * @brief
        * Instruction of subtraction of two arrays and taking plus part using AVX instructions.
        *
        * @param[in] inA
        * 32-byte aligned 1st array as vector.
        *
        * @param[in] inB
        * 32-byte aligned 2nd array as vector which should have same size with inA.
        *
        * @param[out] outC
        * 32-byte aligned resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
        * The size is automatically adjusted to inA and inB in this function.
        */
        void positive_difference_AVX_alvector(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC);

        /**
        * @brief
        * Instruction of subtraction of two arrays and taking plus part using no SIMD instructions.
        *
        * @param[in] inA
        * 32-byte aligned 1st array as vector.
        *
        * @param[in] inB
        * 32-byte aligned 2nd array as vector which should have same size with inA.
        *
        * @param[out] outC
        * 32-byte aligned resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
        * The size is automatically adjusted to inA and inB in this function.
        */
        void positive_difference_alvector(
            const alvector<double>& inA,
            const alvector<double>& inB,
            alvector<double>& outC);

    } // aligned
} // pml

#endif