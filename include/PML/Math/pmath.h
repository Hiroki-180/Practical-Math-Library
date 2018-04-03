#ifndef MATH_PMATH_H
#define MATH_PMATH_H

#pragma once

#include <PML/Core/aligned_vector.h>

namespace pml{

    /**
    * This exp-function is implimented by the algorithm of the following fast math library,
    * @auther herumi
    * @url https://github.com/herumi/fmath
    * @note BSD-3-Clause license.
    */
    double exp(double inX);

    double sin(double inX);

} // pml

#endif