#ifndef CORE_CROSS_INTRIN_H
#define CORE_CROSS_INTRIN_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Functions implemented by SIMD instructions.
*/

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

namespace pml {
    namespace aligned {

        /**
        * @brief
        * Rotate __m256d to the left using SIMD instructions upto AVX.
        *
        * @param[in] inX
        * Target __m256d vector.
        *
        * @param[in] inN
        * Stride. Negative value is interpreted as right rotation.
        *
        * @return
        * Rotated __m256d vector.
        * For instance, if inN = +1 then
        *    { inX[0], inX[1], inX[2], inX[3] } -> { inX[1], inX[2], inX[3], inX[0] }.
        */
        __m256d _mm256_rotate_left_pd(const __m256d& inX, int inN);

    } //aligned
} // pml

#endif