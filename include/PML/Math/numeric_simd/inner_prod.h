#ifndef MATH_NUMERIC_INNER_PROD_H
#define MATH_NUMERIC_INNER_PROD_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml {

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


    namespace aligned {

        /**
        * @fn inner_prod_AVX(const array<double>& inA, const array<double>& inB, std::size_t inSize)
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
        double inner_prod_AVX(
            const alarray<double>& inA,
            const alarray<double>& inB,
            std::size_t inSize);

        /**
        * @fn inner_prod_AVX(const vector32<double>& inA, const vector32<double>& inB)
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
        double inner_prod_AVX(
            const alvector32<double>& inA,
            const alvector32<double>& inB);

    } // aligned
} // pml

#endif