#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

#include <PML/Core/NamespaceMacros.h>

PML_MATH_NS_BEGIN

namespace constants {

    inline double PI() noexcept { return 3.141592653589793; }
    inline double PIHalf() noexcept { return 1.570796326794896; }
    inline double TwoPI() noexcept { return 6.283185307179586; }
    inline double SqrtPI() noexcept { return 1.772453850905515; }
    inline double SqrtPIHalf() noexcept { return 1.253314137315500; }
    inline double SqrtTwoPI() noexcept { return 2.506628274631000; }
    inline double PISqrd() noexcept { return 9.869604401089358; }
    inline double OneOverSqrtTwoPI() noexcept { return 0.3989422804014327; }

    inline double e() noexcept { return 2.718281828459045; }

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
}

PML_MATH_NS_END

#endif