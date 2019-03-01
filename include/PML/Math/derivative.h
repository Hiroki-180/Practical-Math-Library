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

    template<typename F>
    double fwd1stDerivative_1stAcc(F inFunc, double inX, double indX)
    {
        return (-inFunc(inX) + inFunc(inX + indX)) / indX;
    }

    template<typename F>
    double fwd1stDerivative_2ndAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX = inX + indX;

        return (-1.5*inFunc(inX) + 2.0*inFunc(lShiftedX) - 0.5*inFunc(lShiftedX + indX)) / indX;
    }

    template<typename F>
    double fwd1stDerivative_3rdAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX       + indX;
        const auto lShifte2dX = lShiftedX + indX;

        return (-pml::constants::rational_sum(5.0, 6.0, 1.0)*inFunc(inX) + 3.0*inFunc(lShiftedX)
                - 1.5*inFunc(lShifte2dX) + pml::constants::rational(1.0, 3.0)*inFunc(lShifte2dX + indX)) / indX;
    }

    template<typename F>
    double fwd1stDerivative_4thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;

        return (-pml::constants::rational_sum(1.0, 12.0, 2.0)*inFunc(inX) + 4.0*inFunc(lShiftedX)
                - 3.0*inFunc(lShifte2dX) + pml::constants::rational_sum(1.0, 3.0, 1.0)*inFunc(lShifte3dX)
                - 0.25*inFunc(lShifte3dX + indX)) / indX;
    }

    template<typename F>
    double fwd1stDerivative_5thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;

        return (-pml::constants::rational_sum(17.0, 60.0, 2.0)*inFunc(inX) + 5.0*inFunc(lShiftedX)
                - 5.0*inFunc(lShifte2dX) + pml::constants::rational_sum(1.0, 3.0, 3.0)*inFunc(lShifte3dX)
                - 1.25*inFunc(lShifte4dX) + 0.2*inFunc(lShifte4dX + indX)) / indX;
    }

    template<typename F>
    double fwd1stDerivative_6thAcc(F inFunc, double inX, double indX)
    {
        const auto lShiftedX  = inX        + indX;
        const auto lShifte2dX = lShiftedX  + indX;
        const auto lShifte3dX = lShifte2dX + indX;
        const auto lShifte4dX = lShifte3dX + indX;
        const auto lShifte5dX = lShifte4dX + indX;

        return (-2.45*inFunc(inX) + 6.0*inFunc(lShiftedX)
                - 7.5*inFunc(lShifte2dX) + pml::constants::rational_sum(2.0, 3.0, 6.0)*inFunc(lShifte3dX)
                - 3.75*inFunc(lShifte4dX) + 1.2*inFunc(lShifte5dX) - pml::constants::rational(1.0, 6.0)*inFunc(lShifte5dX + indX)) / indX;
    }

    /**
    * Type of finite difference direction.
    */
    enum class DifferenceType
    {
        Forward,
        Central,
        Backward
    };

    /**
    * Order of derivative used in pml::derivative.
    */
    enum class DerivativeOrder
    {
        _1st,
        _2nd,
        _3rd,
        _4th,
        _5th,
        _6th
    };

    /**
    * Accuracy of derivative used in pml::derivative.
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
    double derivative(F inFunc, double inX, double indX, DerivativeOrder inOrder, DifferenceType inType, Accuracy inAccuracy)
    {
        if (inOrder == DerivativeOrder::_1st)
        {
            if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                    return derivative_1st_Fwd_1stAcc(inFunc, inX, indX);
                }
                else if (inAccuracy == Accuracy::_2nd) {
                    return derivative_1st_Fwd_2ndAcc(inFunc, inX, indX);
                }
                else if (inAccuracy == Accuracy::_3rd) {
                    return derivative_1st_Fwd_3rdAcc(inFunc, inX, indX);
                }
                else if (inAccuracy == Accuracy::_4th) {
                    return derivative_1st_Fwd_4thAcc(inFunc, inX, indX);
                }
                else if (inAccuracy == Accuracy::_5th) {
                    return derivative_1st_Fwd_5thAcc(inFunc, inX, indX);
                }
                else if (inAccuracy == Accuracy::_6th) {
                    return derivative_1st_Fwd_6thAcc(inFunc, inX, indX);
                }
            }
            else if (inType == DifferenceType::Central)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
        }
        else if (inOrder == DerivativeOrder::_2nd)
        {
            if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Central)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
        }
        else if (inOrder == DerivativeOrder::_3rd)
        {
            if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Central)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
        }
        else if (inOrder == DerivativeOrder::_4th)
        {
            if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Central)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
        }
        else if (inOrder == DerivativeOrder::_5th)
        {
            if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Central)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
        }
        else if (inOrder == DerivativeOrder::_6th)
        {
            if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Central)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
            else if (inType == DifferenceType::Backward)
            {
                if (inAccuracy == Accuracy::_1st) {
                }
                else if (inAccuracy == Accuracy::_2nd) {
                }
                else if (inAccuracy == Accuracy::_3rd) {
                }
                else if (inAccuracy == Accuracy::_4th) {
                }
                else if (inAccuracy == Accuracy::_5th) {
                }
                else if (inAccuracy == Accuracy::_6th) {
                }
            }
        }

        return 0.0;
    }


} // pml

#endif
