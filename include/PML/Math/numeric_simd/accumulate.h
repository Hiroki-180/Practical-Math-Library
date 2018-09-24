#ifndef MATH_NUMERIC_ACCUMURATE_H
#define MATH_NUMERIC_ACCUMURATE_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml {

    /**
    * @fn accumulate_AVX_array(const double* inA, std::size_t inSize)
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
    double accumulate_AVX_array(
        const double* inA,
        std::size_t inSize);

    /**
    * @fn accumulate_array(const double* inA, std::size_t inSize)
    *
    * @brief
    * Wrapper of std::accumulate.
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
    double accumulate_array(
        const double* inA,
        std::size_t inSize);

    /**
    * @fn accumulate_AVX_vector(const std::vector<double>& inA);
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
    double accumulate_AVX_vector(const std::vector<double>& inA);

    /**
    * @fn accumulate_vector(const std::vector<double>& inA)
    *
    * @brief
    * Wrapper of std::accumulate.
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
    double accumulate_vector(const std::vector<double>& inA);

    namespace aligned {

        /**
        * @fn accumulate_AVX_alvector(const alvector<double>& inA)
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
        double accumulate_AVX_alvector(const alvector<double>& inA);

        /**
        * @fn accumulate_alvector(const alvector<double>& inA)
        *
        * @brief
        * Wrapper of std::accumulate with memory aligned data.
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
        double accumulate_alvector(const alvector<double>& inA);

    } // aligned
} // pml

#endif