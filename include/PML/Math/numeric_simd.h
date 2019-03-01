#ifndef MATH_NUMERIC_SIMD_H
#define MATH_NUMERIC_SIMD_H

/**
* @file
* public header provided by PML.
*
* @brief
* numeric manipulation implemented by SIMD operations.
*/

#include <PML/Core/aligned_allocator.h>

namespace pml {

    /**
    * @brief
    * Accelerated version of std::accumulate by automatically selected optimal SIMD.
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
    double accumulate_SIMD(
        const double* inA,
        std::size_t inSize);

    /**
    * @brief
    * Accelerated version of std::inner_product by automatically selected optimal SIMD.
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
    double inner_product_SIMD(
        const double* inA,
        const double* inB,
        std::size_t inSize);

    /**
    * @brief
    * Instruction of subtraction of two arrays and taking plus part by automatically selected optimal SIMD.
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
    void positive_difference_SIMD(
        const double* inA,
        const double* inB,
        double* outC,
        std::size_t inSize);

    /**
    * @brief
    * Accelerated version of std::accumulate by automatically selected optimal SIMD.
    *
    * @param[in] inA
    * Array as std::vector.
    *
    * @return
    * Sum of the all elements of the input array, inA[0] + inA[1] + ... + inA[inA.size()-1].
    */
    double accumulate_SIMD(const std::vector<double>& inA);

    /**
    * @brief
    * Accelerated version of std::inner_product by automatically selected optimal SIMD.
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
    double inner_product_SIMD(
        const std::vector<double>& inA,
        const std::vector<double>& inB);

    /**
    * @brief
    * Instruction of subtraction of two arrays and taking plus part by automatically selected optimal SIMD.
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
    void positive_difference_SIMD(
        const std::vector<double>& inA,
        const std::vector<double>& inB,
        std::vector<double>& outC);

    namespace aligned {

        /**
        * @brief
        * Accelerated version of std::accumulate by automatically selected optimal SIMD with memory aligned data.
        *
        * @param[in] inA
        * Aligned vector.
        *
        * @return
        * Sum of the all elements of the input array, inA[0] + inA[1] + ... + inA[inA.size()-1].
        */
        double accumulate_SIMD(const pml::aligned::alvector<double>& inA);

        /**
        * @brief
        * Accelerated version of std::inner_product by automatically selected optimal SIMD with memory aligned data.
        *
        * @param[in] inA
        * Aligned 1st vector.
        *
        * @param[in] inB
        * Aligned 2nd vector.
        *
        * @return
        * Inner product of the input arrays, inA[0]*inB[0] + inA[1]*inB[1] + ... + inA[inA.size()-1]*inB[inB.size()-1].
        */
        double inner_product_SIMD(
            const pml::aligned::alvector<double>& inA,
            const pml::aligned::alvector<double>& inB);

        /**
        * @brief
        * Instruction of subtraction of two arrays and taking plus part by automatically selected optimal SIMD with memory aligned data.
        *
        * @param[in] inA
        * Aligned 1st array as vector.
        *
        * @param[in] inB
        * Aligned 2nd array as vector.
        *
        * @param[out] outC
        * Aligned resulted array as vector, (max(inA[0] - inB[0], 0), max(inA[1] - inB[1], 0), ..., max(inA[inA.size()-1] - inB[inA.size()-1], 0)).
        */
        void positive_difference_SIMD(
            const pml::aligned::alvector<double>& inA,
            const pml::aligned::alvector<double>& inB,
            pml::aligned::alvector<double>& outC);

    } // aligned
} // pml

#endif