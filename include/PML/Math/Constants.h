#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

/**
* @def PML_CONST_PI
* Value of PI.
*/
#define PML_CONST_PI 3.141592653589793

/**
* @def PML_CONST_PIHALF
* Value of PI over 2, PI/2.
*/
#define PML_CONST_PIHALF 1.570796326794896

/**
* @def PML_CONST_PIQUATER
* Value of PI over 4, PI/4.
*/
#define PML_CONST_PIQUATER 0.7853981633974483

/**
* @def PML_CONST_TWOPI
* Value of 2 times PI, 2*PI.
*/
#define PML_CONST_TWOPI 6.283185307179586

/**
* @def PML_CONST_SQRTPI
* Value of the square root of PI, sqrt(PI).
*/
#define PML_CONST_SQRTPI 1.772453850905515

/**
* @def PML_CONST_SQRTPIHALF
* Value of the square root of PI over 2, sqrt(PI/2).
*/
#define PML_CONST_SQRTPIHALF 1.253314137315500

/**
* @def PML_CONST_SQRTTWOPI
* Value of the square root of 2 times PI, sqrt(2*PI).
*/
#define PML_CONST_SQRTTWOPI 2.506628274631000

/**
* @def PML_CONST_PISQRD
* Value of the PI squared, PI*PI.
*/
#define PML_CONST_PISQRD 9.869604401089358

/**
* @def PML_CONST_1OVERPI
* Value of the one over PI, 1/PI.
*/
#define PML_CONST_1OVERPI 0.3183098861837907

/**
* @def PML_CONST_2OVERPI
* Value of the two over PI, 2/PI.
*/
#define PML_CONST_2OVERPI 0.6366197723675814

/**
* @def PML_CONST_4OVERPI
* Value of the two over PI, 4/PI.
*/
#define PML_CONST_4OVERPI 1.2732395447351627

/**
* @def PML_CONST_1OVERTWOPI
* Value of the one over 2 times PI, 1/(2*PI).
*/
#define PML_CONST_1OVERTWOPI 0.1591549430918953

/**
* @def PML_CONST_1OVERSQRTTWOPI
* Value of the one over square root of 2 times PI, 1/sqrt(2*PI).
*/
#define PML_CONST_1OVERSQRTTWOPI 0.3989422804014327

/**
* @def PML_CONST_E
* Value of Napire's number.
*/
#define PML_CONST_E 2.718281828459045

/**
* @namespace pml
*
* @brief
* Top level of the Practical Math Library.
*/
namespace pml {

    /**
    * @namespace constants
    *
    * @brief
    * List of mathematical constants.
    */
    namespace constants {

        inline double PI() noexcept { return PML_CONST_PI; }
        inline double PIHalf() noexcept { return PML_CONST_PIHALF; }
        inline double PIQuater() noexcept { return PML_CONST_PIQUATER; }
        inline double TwoPI() noexcept { return PML_CONST_TWOPI; }
        inline double SqrtPI() noexcept { return PML_CONST_SQRTPI; }
        inline double SqrtPIHalf() noexcept { return PML_CONST_SQRTPIHALF; }
        inline double SqrtTwoPI() noexcept { return PML_CONST_SQRTTWOPI; }
        inline double PISqrd() noexcept { return PML_CONST_PISQRD; }
        inline double _1OverPI() noexcept { return PML_CONST_1OVERPI; }
        inline double _2OverPI() noexcept { return PML_CONST_2OVERPI; }
        inline double _4OverPI() noexcept { return PML_CONST_4OVERPI; }
        inline double _1OverTwoPI() noexcept { return PML_CONST_1OVERTWOPI; }
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

        /**
        * @namespace Q
        *
        * @brief
        * List of rational numbers.
        */
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