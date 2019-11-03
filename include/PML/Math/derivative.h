#ifndef MATH_DERIVATIVE_H
#define MATH_DERIVATIVE_H

#include <PML/Math/constants.h>
#include <cmath>

/**
* @file
* public header provided by PML.
*
* @brief
* Numeric derivative with various precision.
*/

namespace pml{

    namespace derivative {

        /**
        * @brief Type of finite difference direction.
        */
        enum class Type
        {
            Forward,
            Central,
            Backward
        };

        /**
        * @brief Order of derivative used in pml::derivative.
        */
        enum class Order
        {
            _1st,
            _2nd,
            _3rd,
            _4th,
            _5th,
            _6th
        };

        /**
        * @brief Accuracy of derivative used in pml::derivative.
        */
        enum class Accuracy
        {
            _1st,
            _2nd,
            _3rd,
            _4th,
            _5th,
            _6th
        };
    }

    template<class F>
    constexpr double derivative_1st_fwd_1stAcc(F inFunc, double inX, double indX)
    {
        return (-inFunc(inX) + inFunc(inX + indX)) / indX;
    }

    template<class F>
    constexpr double derivative_1st_fwd_2ndAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX = inX + indX;

        return (-1.5*inFunc(inX) + 2.0*inFunc(lShiftedX) - 0.5*inFunc(lShiftedX + indX)) / indX;
    }

    template<class F>
    constexpr double derivative_1st_fwd_3rdAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;

        return (- pml::constants::divide(11.0, 6.0) * inFunc(inX)
                + 3.0                               * inFunc(lShiftedX)
                - 1.5                               * inFunc(lShifte2dX)
                + pml::constants::divide(1.0, 3.0)  * inFunc(lShifte3dX)) / indX;
    }

    template<class F>
    constexpr double derivative_1st_fwd_4thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;

        return (- pml::constants::divide(25.0, 12.0) * inFunc(inX)
                + 4.0                                * inFunc(lShiftedX)
                - 3.0                                * inFunc(lShifte2dX)
                + pml::constants::divide(4.0, 3.0)   * inFunc(lShifte3dX)
                - 0.25                               * inFunc(lShifte4dX)) / indX;
    }

    template<class F>
    constexpr double derivative_1st_fwd_5thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;
        const auto lShifte5dX = lShifte4dX + indX;

        return (- pml::constants::divide(137.0, 60.0) * inFunc(inX)
                + 5.0                                 * inFunc(lShiftedX)
                - 5.0                                 * inFunc(lShifte2dX)
                + pml::constants::divide(10.0, 3.0)   * inFunc(lShifte3dX)
                - 1.25                                * inFunc(lShifte4dX) 
                + 0.2                                 * inFunc(lShifte5dX)) / indX;
    }

    template<class F>
    constexpr double derivative_1st_fwd_6thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;
        const auto lShifte5dX = lShifte4dX + indX;
        const auto lShifte6dX = lShifte5dX + indX;

        return (- 2.45                              * inFunc(inX) 
                + 6.0                               * inFunc(lShiftedX)
                - 7.5                               * inFunc(lShifte2dX)
                + pml::constants::divide(20.0, 3.0) * inFunc(lShifte3dX)
                - 3.75                              * inFunc(lShifte4dX)
                + 1.2                               * inFunc(lShifte5dX)
                - pml::constants::divide(1.0, 6.0)  * inFunc(lShifte6dX)) / indX;
    }

    template<class F>
    constexpr double derivative_2nd_fwd_1stAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX = inX + indX;

        return (inFunc(inX) - 2.0 * inFunc(lShiftedX) + inFunc(lShiftedX + indX)) / (indX * indX);
    }

    template<class F>
    constexpr double derivative_2nd_fwd_2ndAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;

        return (2.0 * inFunc(inX) - 5.0 * inFunc(lShiftedX) + 4.0 * inFunc(lShifte2dX) - inFunc(lShifte2dX + indX)) / (indX * indX);
    }

    template<class F>
    constexpr double derivative_2nd_fwd_3rdAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;

        return (  pml::constants::divide(35.0, 12.0) * inFunc(inX) 
                - pml::constants::divide(26.0,  3.0) * inFunc(lShiftedX)
                + 9.5                                * inFunc(lShifte2dX) 
                - pml::constants::divide(14.0,  3.0) * inFunc(lShifte3dX)
                + pml::constants::divide(11.0, 12.0) * inFunc(lShifte4dX)) / (indX * indX);
    }

    template<class F>
    constexpr double derivative_2nd_fwd_4thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;
        const auto lShifte5dX = lShifte4dX + indX;

        return (  pml::constants::divide( 15.0, 4.0) * inFunc(inX)
                - pml::constants::divide( 77.0, 6.0) * inFunc(lShiftedX)
                + pml::constants::divide(107.0, 6.0) * inFunc(lShifte2dX)
                - 13.0                               * inFunc(lShifte3dX)
                + pml::constants::divide(61.0, 12.0) * inFunc(lShifte4dX)
                - pml::constants::divide( 5.0,  6.0) * inFunc(lShifte5dX)) / (indX * indX);
    }

    template<class F>
    constexpr double derivative_2nd_fwd_5thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX = inX + indX;
        const auto lShifte2dX = lShiftedX + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;
        const auto lShifte5dX = lShifte4dX + indX;
        const auto lShifte6dX = lShifte5dX + indX;

        return (  pml::constants::divide(203.0, 45.0)  * inFunc(inX) 
                - pml::constants::divide( 87.0,  5.0)  * inFunc(lShiftedX)
                + pml::constants::divide(117.0,  4.0)  * inFunc(lShifte2dX)
                - pml::constants::divide(254.0,  9.0)  * inFunc(lShifte3dX)
                + 16.5 * inFunc(lShifte4dX)
                -  5.4 * inFunc(lShifte5dX)
                + pml::constants::divide(137.0, 180.0) * inFunc(lShifte6dX)) / (indX * indX);
    }

    template<class F>
    constexpr double derivative_2nd_fwd_6thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX = inX + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;
        const auto lShifte5dX = lShifte4dX + indX;
        const auto lShifte6dX = lShifte5dX + indX;
        const auto lShifte7dX = lShifte6dX + indX;

        return (  pml::constants::divide(469.0, 90.0)   * inFunc(inX) 
                - 22.3                                  * inFunc(lShiftedX)
                + pml::constants::divide(879.0,  20.0)  * inFunc(lShifte2dX)
                - pml::constants::divide(949.0,  18.0)  * inFunc(lShifte3dX)
                + 41.0                                  * inFunc(lShifte4dX)
                - 20.1                                  * inFunc(lShifte5dX)
                + pml::constants::divide(1019.0, 180.0) * inFunc(lShifte6dX)
                - 0.7                                   * inFunc(lShifte7dX)) / (indX * indX);
    }

    /**
    * @brief
    * Calculate numeric derivatives of one dimensional functions f(x).
    *
    * @param[in] inFunc
    * One dimentiocal function f.
    *
    * @param[in] inX
    * Value of the argument of derivative of f.
    *
    * @param[in] indX
    * Shift width in finite difference of f.
    *
    * @param[in] inOrder
    * Order of derivative.
    *
    * @param[in] inType
    * Type of finite difference.
    *
    * @param[in] inAccuracy
    * Required accuracy.
    *
    * @return f^(n) (x)
    */
    template<typename F>
    double calc(F inFunc, double inX, double indX, derivative::Order inOrder, derivative::Type inType, derivative::Accuracy inAccuracy)
    {
        return 0.0;
    }


} // pml

#endif
