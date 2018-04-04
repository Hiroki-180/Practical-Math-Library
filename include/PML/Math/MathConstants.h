#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

#define PML_CONST_PI             3.141592653589793
#define PML_CONST_PIHALH         1.570796326794896
#define PML_CONST_TWOPI          6.283185307179586
#define PML_CONST_SQRTPI         1.772453850905515
#define PML_CONST_SQRTPIHALF     1.253314137315500
#define PML_CONST_SQRTTWOPI      2.506628274631000
#define PML_CONST_PISQRD         9.869604401089358
#define PML_CONST_1OVERPI        0.1591549430918953
#define PML_CONST_1OVERSQRTTWOPI 0.3989422804014327

#define PML_CONST_E 2.718281828459045

namespace pml {
    namespace constants {

        inline double PI() noexcept { return PML_CONST_PI; }
        inline double PIHalf() noexcept { return PML_CONST_PIHALH; }
        inline double TwoPI() noexcept { return PML_CONST_TWOPI; }
        inline double SqrtPI() noexcept { return PML_CONST_SQRTPI; }
        inline double SqrtPIHalf() noexcept { return PML_CONST_SQRTPIHALF; }
        inline double SqrtTwoPI() noexcept { return PML_CONST_SQRTTWOPI; }
        inline double PISqrd() noexcept { return PML_CONST_PISQRD; }
        inline double _1OverTwoPI() noexcept { return PML_CONST_1OVERPI; }
        inline double _1OverSqrtTwoPI() noexcept { return PML_CONST_1OVERSQRTTWOPI; }

        inline double e() noexcept { return PML_CONST_E; }

        inline double SqrtOf2() noexcept { return 1.414213562373095; }
        inline double SqrtOf3() noexcept { return 1.732050807568877; }
        inline double SqrtOf5() noexcept { return 2.236067977499789; }
        inline double SqrtOf6() noexcept { return 2.449489742783178; }
        inline double SqrtOf7() noexcept { return 2.645751311064590; }
        inline double SqrtOf8() noexcept { return 2.828427124746190; }
        inline double SqrtOf10() noexcept { return 3.162277660168379; }

        inline double GoldenRatio() noexcept { return 1.618033988749894; }
        inline double Apery() noexcept { return 1.202056903159594; }
        inline double UniversalParabolicP() noexcept { return 2.295587149392638; }

        // rational numbers.
        namespace Q
        {
            inline double _1Over3() noexcept { return 0.3333333333333333; }
            inline double _1Over6() noexcept { return 0.1666666666666666; }
            inline double _1Over7() noexcept { return 0.1428571428571428; }
            inline double _1Over9() noexcept { return 0.1111111111111111; }
        }

    } // constants
} //pml

#endif