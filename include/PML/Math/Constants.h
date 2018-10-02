#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* List of mathematical constants.
*/

#include <limits>

/**
* @def PML_CONST_PI
*
* @brief
* Value of PI.
*/
#define PML_CONST_PI 3.141592653589793238463

/**
* @def PML_CONST_PIHALF
*
* @bries
* Value of PI over 2, PI/2.
*/
#define PML_CONST_PIHALF 1.570796326794896619231

/**
* @def PML_CONST_PIQUATER
*
* @brief
* Value of PI over 4, PI/4.
*/
#define PML_CONST_PIQUATER 0.7853981633974483096157

/**
* @def PML_CONST_TWOPI
*
* @brief
* Value of 2 times PI, 2*PI.
*/
#define PML_CONST_TWOPI 6.283185307179586476925

/**
* @def PML_CONST_SQRTPI
*
* @brief
* Value of the square root of PI, sqrt(PI).
*/
#define PML_CONST_SQRTPI 1.772453850905516027298

/**
* @def PML_CONST_SQRTPIHALF
*
* @brief
* Value of the square root of PI over 2, sqrt(PI/2).
*/
#define PML_CONST_SQRTPIHALF 1.253314137315500251208

/**
* @def PML_CONST_SQRTTWOPI
*
* @brief
* Value of the square root of 2 times PI, sqrt(2*PI).
*/
#define PML_CONST_SQRTTWOPI 2.506628274631000502416

/**
* @def PML_CONST_PISQRD
*
* @brief
* Value of the PI squared, PI*PI.
*/
#define PML_CONST_PISQRD 9.869604401089358618834

/**
* @def PML_CONST_1OVERPI
*
* @brief
* Value of the one over PI, 1/PI.
*/
#define PML_CONST_1OVERPI 0.3183098861837906715378

/**
* @def PML_CONST_2OVERPI
*
* @brief
* Value of the two over PI, 2/PI.
*/
#define PML_CONST_2OVERPI 0.6366197723675813430755

/**
* @def PML_CONST_4OVERPI
*
* @brief
* Value of the four over PI, 4/PI.
*/
#define PML_CONST_4OVERPI 1.273239544735162686151

/**
* @def PML_CONST_1OVERTWOPI
*
* @brief
* Value of the one over 2 times PI, 1/(2*PI).
*/
#define PML_CONST_1OVERTWOPI 0.1591549430918953357689

/**
* @def PML_CONST_1OVERSQRTTWOPI
*
* @brief
* Value of the one over square root of 2 times PI, 1/sqrt(2*PI).
*/
#define PML_CONST_1OVERSQRTTWOPI 0.3989422804014326779399

/**
* @def PML_CONST_E
*
* @brief
* Value of the Napire's number.
*/
#define PML_CONST_E 2.71828182845904523536

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

        namespace detail {

            double constexpr sqrtNewtonRaphson(double x, double curr, double prev)
            {
                return curr == prev
                    ? curr
                    : pml::constants::detail::sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
            }
        }

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

        /*
        * @brief
        * Constexpr version of std::fabs.
        *
        * @return
        * inA * inB.
        */
        constexpr double fabs(double inX)
        {
            return (inX >= 0.0) ? inX : -inX;
        }

        /*
        * @brief
        * Constexpr version of sum(+).
        *
        * @return
        * inA * inB.
        */
        constexpr double sum(double inA, double inB)
        {
            return inA + inB;
        }

        /*
        * @brief
        * Constexpr version of multiplication(*).
        *
        * @return
        * inA * inB.
        */
        constexpr double multiply(double inA, double inB)
        {
            return inA * inB;
        }

        /*
        * @brief
        * Constexpr version of the rational number(/).
        *
        * @return
        * inNumerator / inDenominator.
        */
        constexpr double rational(double inNumerator, double inDenominator)
        {
            return inNumerator / inDenominator;
        }

        /*
        * @brief
        * Constexpr version of the rational number plus constant.
        *
        * @return
        * (inNumerator / inDenominator) + inX.
        */
        constexpr double rational_sum(double inNumerator, double inDenominator, double inX)
        {
            return (inNumerator / inDenominator) + inX;
        }

        /*
        * @brief
        * Constexpr version of the square root.
        *
        * @return
        * Square root of inX.
        */
        double constexpr sqrt(double inX)
        {
            return ((inX >= 0) && (inX < std::numeric_limits<double>::infinity()))
                ? pml::constants::detail::sqrtNewtonRaphson(inX, inX, 0)
                : std::numeric_limits<double>::quiet_NaN();
        }

        /*
        * @brief
        * Constexpr version of the power.
        *
        * @return
        * std::power(inX, inExponent).
        */
        constexpr double pow(double inX, unsigned int inExponent)
        {
            return (inExponent == 0)     ? 1.0 :
                   (inExponent % 2 == 0) ? pow(inX, inExponent / 2)*pow(inX, inExponent / 2) :
                   inX * pow(inX, (inExponent - 1) / 2) * pow(inX, (inExponent - 1) / 2);
        }

    } // constants
} //pml

#endif