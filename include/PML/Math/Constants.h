#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

/**
* @def PML_CONST_PI
*
* @brief
* Value of PI.
*/
#define PML_CONST_PI 3.141592653589793

/**
* @def PML_CONST_PIHALF
*
* @bries
* Value of PI over 2, PI/2.
*/
#define PML_CONST_PIHALF 1.570796326794896

/**
* @def PML_CONST_PIQUATER
*
* @brief
* Value of PI over 4, PI/4.
*/
#define PML_CONST_PIQUATER 0.7853981633974483

/**
* @def PML_CONST_TWOPI
*
* @brief
* Value of 2 times PI, 2*PI.
*/
#define PML_CONST_TWOPI 6.283185307179586

/**
* @def PML_CONST_SQRTPI
*
* @brief
* Value of the square root of PI, sqrt(PI).
*/
#define PML_CONST_SQRTPI 1.772453850905515

/**
* @def PML_CONST_SQRTPIHALF
*
* @brief
* Value of the square root of PI over 2, sqrt(PI/2).
*/
#define PML_CONST_SQRTPIHALF 1.253314137315500

/**
* @def PML_CONST_SQRTTWOPI
*
* @brief
* Value of the square root of 2 times PI, sqrt(2*PI).
*/
#define PML_CONST_SQRTTWOPI 2.506628274631000

/**
* @def PML_CONST_PISQRD
*
* @brief
* Value of the PI squared, PI*PI.
*/
#define PML_CONST_PISQRD 9.869604401089358

/**
* @def PML_CONST_1OVERPI
*
* @brief
* Value of the one over PI, 1/PI.
*/
#define PML_CONST_1OVERPI 0.3183098861837907

/**
* @def PML_CONST_2OVERPI
*
* @brief
* Value of the two over PI, 2/PI.
*/
#define PML_CONST_2OVERPI 0.6366197723675814

/**
* @def PML_CONST_4OVERPI
*
* @brief
* Value of the four over PI, 4/PI.
*/
#define PML_CONST_4OVERPI 1.2732395447351627

/**
* @def PML_CONST_1OVERTWOPI
*
* @brief
* Value of the one over 2 times PI, 1/(2*PI).
*/
#define PML_CONST_1OVERTWOPI 0.1591549430918953

/**
* @def PML_CONST_1OVERSQRTTWOPI
*
* @brief
* Value of the one over square root of 2 times PI, 1/sqrt(2*PI).
*/
#define PML_CONST_1OVERSQRTTWOPI 0.3989422804014327

/**
* @def PML_CONST_E
*
* @brief
* Value of the Napire's number.
*/
#define PML_CONST_E 2.718281828459045

/**
* @namespace pml
*
* @brief
* Top level of the Practical Math Library.
* Basic functions and algorithms of this library are provided in this namespace.
*/
namespace pml {

    /**
    * @namespace constants
    *
    * @brief
    * List of mathematical constants, PI, e, and so on.
    */
    namespace constants {

        /**
        * @brief
        * Inline function returning the value of PI.
        *
        * @return
        * Value of PI.
        */
        inline double PI() noexcept { return PML_CONST_PI; }

        /**
        * @brief
        * Inline function returning the value of PI over 2.
        *
        * @return
        * Value of PI/2.
        */
        inline double PIHalf() noexcept { return PML_CONST_PIHALF; }

        /**
        * @brief
        * Inline function returning the value of PI over 4.
        *
        * @return
        * Value of PI/4.
        */
        inline double PIQuater() noexcept { return PML_CONST_PIQUATER; }

        /**
        * @brief
        * Inline function returning the value of 2 times PI.
        *
        * @return
        * Value of 2*PI.
        */
        inline double TwoPI() noexcept { return PML_CONST_TWOPI; }

        /**
        * @brief
        * Inline function returning the value of the square root of PI.
        *
        * @return
        * Value of sqrt(PI).
        */
        inline double SqrtPI() noexcept { return PML_CONST_SQRTPI; }

        /**
        * @brief
        * Inline function returning the value of the square root of PI over 2.
        *
        * @return
        * Value of sqrt(PI/2).
        */
        inline double SqrtPIHalf() noexcept { return PML_CONST_SQRTPIHALF; }

        /**
        * @brief
        * Inline function returning the value of the square root of 2 times PI.
        *
        * @return
        * Value of sqrt(2*PI).
        */
        inline double SqrtTwoPI() noexcept { return PML_CONST_SQRTTWOPI; }

        /**
        * @brief
        * Inline function returning the value of PI squared.
        *
        * @return
        * Value of PI*PI.
        */
        inline double PISqrd() noexcept { return PML_CONST_PISQRD; }

        /**
        * @brief
        * Inline function returning the value of 1 over PI.
        *
        * @return
        * Value of 1/PI.
        */
        inline double _1OverPI() noexcept { return PML_CONST_1OVERPI; }

        /**
        * @brief
        * Inline function returning the value of 2 over PI.
        *
        * @return
        * Value of 2/PI.
        */
        inline double _2OverPI() noexcept { return PML_CONST_2OVERPI; }

        /**
        * @brief
        * Inline function returning the value of 4 over PI.
        *
        * @return
        * Value of 4/PI.
        */
        inline double _4OverPI() noexcept { return PML_CONST_4OVERPI; }

        /**
        * @brief
        * Inline function returning the value of 1 over 2 times PI.
        *
        * @return
        * Value of 1/(2*PI).
        */
        inline double _1OverTwoPI() noexcept { return PML_CONST_1OVERTWOPI; }

        /**
        * @brief
        * Inline function returning the value of 1 over the square root of 2 times PI.
        *
        * @return
        * Value of 1/sqrt(2*PI).
        */
        inline double _1OverSqrtTwoPI() noexcept { return PML_CONST_1OVERSQRTTWOPI; }

        /**
        * @brief
        * Inline function returning the value of the Nepire's number.
        *
        * @return
        * Value of e.
        */
        inline double e() noexcept { return PML_CONST_E; }

        /**
        * @brief
        * Inline function returning the value of the square root of 2.
        *
        * @return
        * Value of sqrt(2.0).
        */
        inline double SqrtOf2() noexcept { return 1.414213562373095; }

        /**
        * @brief
        * Inline function returning the value of the square root of 3.
        *
        * @return
        * Value of sqrt(3.0).
        */
        inline double SqrtOf3() noexcept { return 1.732050807568877; }

        /**
        * @brief
        * Inline function returning the value of the square root of 5.
        *
        * @return
        * Value of sqrt(5.0).
        */
        inline double SqrtOf5() noexcept { return 2.236067977499789; }

        /**
        * @brief
        * Inline function returning the value of the square root of 6.
        *
        * @return
        * Value of sqrt(6.0).
        */
        inline double SqrtOf6() noexcept { return 2.449489742783178; }

        /**
        * @brief
        * Inline function returning the value of the square root of 7.
        *
        * @return
        * Value of sqrt(7.0).
        */
        inline double SqrtOf7() noexcept { return 2.645751311064590; }

        /**
        * @brief
        * Inline function returning the value of the square root of 8.
        *
        * @return
        * Value of sqrt(8.0).
        */
        inline double SqrtOf8() noexcept { return 2.828427124746190; }

        /**
        * @brief
        * Inline function returning the value of the square root of 10.
        *
        * @return
        * Value of sqrt(10.0).
        */
        inline double SqrtOf10() noexcept { return 3.162277660168379; }

        /**
        * @brief
        * Inline function returning the value of the golden ratio.
        *
        * @return
        * Value of the golden ratio, 1.618033988749894.
        */
        inline double GoldenRatio() noexcept { return 1.618033988749894; }

        /**
        * @brief
        * Inline function returning the value of the Apery's constant.
        * This is also equal to the value of the Riemann zeta fanction of 3. 
        *
        * @return
        * Value of the Apery's constant, 1.202056903159594.
        */
        inline double Apery() noexcept { return 1.202056903159594; }

        /**
        * @brief
        * Inline function returning the value of the universal palabolic constant, ln(1 + sqrt(2.0)) + sqrt(2.0).
        *
        * @return
        * Value of the universal palabolic constant, 1.618033988749894.
        */
        inline double UniversalParabolicP() noexcept { return 2.295587149392638; }

        /**
        * @namespace Q
        *
        * @brief
        * List of rational numbers, 1/3, 1/6, and so on.
        */
        namespace Q
        {
            /**
            * @brief
            * Inline function returning the value of 1 over 3.
            *
            * @return
            * Value of 1.0/3.0.
            */
            inline double _1over3() noexcept { return 0.3333333333333333; }

            /**
            * @brief
            * Inline function returning the value of 1 over 6.
            *
            * @return
            * Value of 1.0/6.0.
            */
            inline double _1over6() noexcept { return 0.1666666666666666; }

            /**
            * @brief
            * Inline function returning the value of 1 over 7.
            *
            * @return
            * Value of 1.0/7.0.
            */
            inline double _1over7() noexcept { return 0.1428571428571428; }

            /**
            * @brief
            * Inline function returning the value of 1 over 9.
            *
            * @return
            * Value of 1.0/9.0.
            */
            inline double _1over9() noexcept { return 0.1111111111111111; }
        }// Q

    } // constants
} //pml

#endif