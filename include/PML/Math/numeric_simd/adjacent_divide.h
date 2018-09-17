#ifndef MATH_NUMERIC_ADJACENT_DIVIDE_H
#define MATH_NUMERIC_ADJACENT_DIVIDE_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml {

    /**
    * @fn adjacent_divide_AVX(const double* inA, double* outB, std::size_t inSize)
    *
    * @brief
    * Dividing elements by the adjacent elements. Still unstable.
    *
    * @param[in] inA
    * Pointer to the address of the first element of the input array.
    *
    * @param[in] inShift
    * Shift value.
    *
    * @param[out] outB
    * Resulted array, (inA[0], inA[0]/inA[1] + inShift, inA[1]/inA[2] + inShift, ..., inA[inSize-2]/inA[inSize-1] + inShift).
    *
    * @param[in] inSize
    * Size of the array inA and outB.
    */
    void adjacent_divide_AVX(
        const double* inA,
        double inShift,
        double* outB,
        std::size_t inSize);

    /**
    * @fn adjacent_divide_AVX(const std::vector<double>& inA, std::vector<double>& outB)
    *
    * @brief
    * Dividing elements by the adjacent elements. Still unstable.
    *
    * @param[in] inA
    * Input array as std::vector.
    *
    * @param[in] inShift
    * Shift value.
    *
    * @param[out] outB
    * Resulted array as vector, (inA[0], inA[0]/inA[1] + inShift, inA[1]/inA[2] + inShift, ..., inA[inSize-2]/inA[inSize-1] + inShift).
    */
    void adjacent_divide_AVX(
        const std::vector<double>& inA,
        double inShift,
        std::vector<double>& outB);


    namespace aligned {

        /**
        * @fn adjacent_divide_AVX(const alvector<double>& inA, double inShift, alvector<double>& outB)
        *
        * @brief
        * Dividing elements by the adjacent elements using AVX and memory aligned data. Still unstable.
        *
        * @param[in] inA
        * 32-byte aligned input array as vector.
        *
        * @param[in] inShift
        * Shift value.
        *
        * @param[out] outB
        * 32-byte aligned resulted array as vector, (inA[0], inA[0]/inA[1] + inShift, inA[1]/inA[2] + inShift, ..., inA[inSize-2]/inA[inSize-1] + inShift).
        */
        void adjacent_divide_AVX(
            const alvector<double>& inA,
            double inShift,
            alvector<double>& outB);

    } // aligned
} // pml

#endif