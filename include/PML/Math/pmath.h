#ifndef MATH_PMATH_H
#define MATH_PMATH_H

#pragma once

namespace pml{

    /**
    * @fn double exp(double inX)
    *
    * @brief
    * Exp-function implimented by look-up-table and Taylor expansion.
    * This exp-function is implimented by the algorithm of the following fast math library,
    *  @auther herumi
    *  @url https://github.com/herumi/fmath
    *  @note BSD-3-Clause license.
    *
    * @param[in] inX
    * argument
    *
    * @return exp(inX)
    */
    double exp(double inX);

    /**
    * @fn double sin(double inX)
    *
    * @brief
    * Sin-function implimented by look-up-table and Taylor expansion.
    *
    * @param[in] inX
    * argument
    *
    * @return sin(inX)
    */
    double sin(double inX);

    /**
    * @fn double sin(double inX)
    *
    * @brief
    * Cos-function implimented by look-up-table and Taylor expansion.
    *
    * @param[in] inX
    * argument
    *
    * @return cos(inX)
    */
    double cos(double inX);

} // pml

#endif