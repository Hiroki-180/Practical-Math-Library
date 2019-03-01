#ifndef CORE_CROSS_INTRIN_H
#define CORE_CROSS_INTRIN_H

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
        inline __m256d _mm256_rotate_left_pd(const __m256d& inX, int inN)
        {
            const __m128i c  = _mm_cvtsi32_si128(inN);
            const __m128i cc = _mm_unpacklo_epi64(c, c);

            // create blend masks (highest bit)
            const __m128d half_low = _mm_castsi128_pd(_mm_slli_epi64(cc, 63));
            const __m128d swap_low = _mm_castsi128_pd(_mm_slli_epi64(cc, 62));
            const __m256d half = _mm256_insertf128_pd(_mm256_castpd128_pd256(half_low), half_low, 1);
            const __m256d swap = _mm256_insertf128_pd(_mm256_castpd128_pd256(swap_low), swap_low, 1);

            // compute rotations
            const __m256d t0 = _mm256_permute_pd(inX, 0x05);           // [2 3 0 1]
            const __m256d t1 = _mm256_permute2f128_pd(t0, t0, 0x01);   // [1 0 2 3]

            const __m256d y0 = inX;                                    // [3 2 1 0]
            const __m256d y1 = _mm256_blend_pd(t0, t1, 0x0a);          // [0 3 2 1]
            const __m256d y2 = _mm256_permute2f128_pd(inX, inX, 0x01); // [1 0 3 2]
            const __m256d y3 = _mm256_blend_pd(t0, t1, 0x05);          // [2 1 0 3]

            // select correct rotation
            const __m256d y01 = _mm256_blendv_pd(y0, y1, half);
            const __m256d y23 = _mm256_blendv_pd(y2, y3, half);
            const __m256d yn  = _mm256_blendv_pd(y01, y23, swap);

            return yn;
        }
    } //aligned
} // pml

#endif