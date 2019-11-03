#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/constants.h>
#include <cmath>

TEST(constants, PI)
{
    EXPECT_DOUBLE_EQ(acos(-1.0), pml::constants::PI());
}

TEST(constants, PI_Relations)
{
    const auto lAnsPI = std::acos(-1.0);
    EXPECT_DOUBLE_EQ(std::sqrt(lAnsPI), pml::constants::SqrtPI());
    EXPECT_DOUBLE_EQ(1.0 / lAnsPI, pml::constants::_1OverPI());
    EXPECT_DOUBLE_EQ(2.0 / lAnsPI, pml::constants::_2OverPI());
    EXPECT_DOUBLE_EQ(4.0 / lAnsPI, pml::constants::_4OverPI());

    const auto lAnsPIHalf = 0.5*lAnsPI;
    EXPECT_DOUBLE_EQ(lAnsPIHalf, pml::constants::PIHalf());
    EXPECT_DOUBLE_EQ(std::sqrt(lAnsPIHalf), pml::constants::SqrtPIHalf());

    const auto lAnsPIQuater = 0.25*lAnsPI;
    EXPECT_DOUBLE_EQ(lAnsPIQuater, pml::constants::PIQuater());

    const auto lAnsTwoPI = 2.0*lAnsPI;
    EXPECT_DOUBLE_EQ(lAnsTwoPI, pml::constants::TwoPI());
    EXPECT_DOUBLE_EQ(std::sqrt(lAnsTwoPI), pml::constants::SqrtTwoPI());
    EXPECT_DOUBLE_EQ(1.0 / lAnsTwoPI, pml::constants::_1OverTwoPI());
    EXPECT_DOUBLE_EQ(1.0 / std::sqrt(lAnsTwoPI), pml::constants::_1OverSqrtTwoPI());

    EXPECT_DOUBLE_EQ(lAnsPI*lAnsPI, pml::constants::PISqrd());
}

TEST(constants, e)
{
    EXPECT_DOUBLE_EQ(1.0, std::log(pml::constants::e()));
}

TEST(constants, GoldenRatio)
{
    EXPECT_DOUBLE_EQ(0.5*(1.0 + pml::constants::sqrt(5.0)), pml::constants::GoldenRatio());
}

TEST(constants, Apery)
{
    double lRiemannZeta3 = 0.0;
    for (auto i = 20000000; i > 0; --i)
    {
        const double lDen = (static_cast<double>(i)*i*i);
        lRiemannZeta3 += 1.0 / lDen;
    }

    EXPECT_DOUBLE_EQ(lRiemannZeta3, pml::constants::Apery());
}

TEST(constants, UniversalParabolicP)
{
    EXPECT_DOUBLE_EQ(
        std::log(1.0 + pml::constants::sqrt(2.0)) + pml::constants::sqrt(2.0),
        pml::constants::UniversalParabolicP());
}

TEST(constants, abs)
{
    static_assert((pml::constants::fabs( 1.0) == 1.0), "Err. of pml::constants::fabs( 1.0)");
    static_assert((pml::constants::fabs(-1.0) == 1.0), "Err. of pml::constants::fabs(-1.0)");
    static_assert((pml::constants::fabs( 5.0) == 5.0), "Err. of pml::constants::fabs( 5.0)");
    static_assert((pml::constants::fabs(-5.0) == 5.0), "Err. of pml::constants::fabs(-5.0)");
}

TEST(constants, sum)
{
    static_assert((pml::constants::sum( 1.0, 1.0) ==  2.0), "Err. of pml::constants::sum( 1.0, 1.0)");
    static_assert((pml::constants::sum(-1.0, 1.0) ==  0.0), "Err. of pml::constants::sum(-1.0, 1.0)");
    static_assert((pml::constants::sum( 8.0, 3.0) == 11.0), "Err. of pml::constants::sum( 8.0, 3.0)");
    static_assert((pml::constants::sum(-5.0, 9.0) ==  4.0), "Err. of pml::constants::sum(-5.0, 9.0)");
}

TEST(constants, multiply)
{
    static_assert((pml::constants::multiply( 1.0, 1.0) ==   1.0), "Err. of pml::constants::multiply( 1.0, 1.0)");
    static_assert((pml::constants::multiply(-1.0, 1.0) ==  -1.0), "Err. of pml::constants::multiply(-1.0, 1.0)");
    static_assert((pml::constants::multiply( 8.0, 3.0) ==  24.0), "Err. of pml::constants::multiply( 8.0, 3.0)");
    static_assert((pml::constants::multiply(-5.0, 9.0) == -45.0), "Err. of pml::constants::multiply(-5.0, 9.0)");
}

TEST(constants, rational)
{
    static_assert((pml::constants::divide(1.0, 3.0) == 0.3333333333333333333333), "Err. of pml::constants::divide(1.0, 3.0)");
    static_assert((pml::constants::divide(1.0, 6.0) == 0.1666666666666666666667), "Err. of pml::constants::divide(1.0, 6.0)");
    static_assert((pml::constants::divide(1.0, 7.0) == 0.1428571428571428571429), "Err. of pml::constants::divide(1.0, 7.0)");
    static_assert((pml::constants::divide(1.0, 9.0) == 0.1111111111111111111111), "Err. of pml::constants::divide(1.0, 9.0)");
}

TEST(constants, rational_sum)
{
    static_assert((pml::constants::divsum(1.0, 3.0,  2.0) ==  2.33333333333333333), "Err. of pml::constants::divsum(1, 3, 2)");
    static_assert((pml::constants::divsum(1.0, 6.0, -3.0) == -2.83333333333333333), "Err. of pml::constants::divsum(1, 6, -3)");
    static_assert((pml::constants::divsum(5.0, 2.0,  1.0) ==  3.5), "Err. of pml::constants::divsum(5, 2, 1)");
    static_assert((pml::constants::divsum(8.0, 9.0, -5.0) == -4.11111111111111111), "Err. of pml::constants::divsum(8, 9, -5)");
}

TEST(constants, sqrt)
{
    static_assert(pml::constants::fabs(pml::constants::sqrt(2.0)  - 1.41421356237309504880) < 1.0e-15, "Err. of pml::constants::sqrt(2.0).");
    static_assert(pml::constants::fabs(pml::constants::sqrt(3.0)  - 1.73205080756887729352) < 1.0e-15, "Err. of pml::constants::sqrt(3.0).");
    static_assert(pml::constants::fabs(pml::constants::sqrt(5.0)  - 2.23606797749978969640) < 1.0e-15, "Err. of pml::constants::sqrt(5.0).");
    static_assert(pml::constants::fabs(pml::constants::sqrt(6.0)  - 2.44948974278317809819) < 1.0e-15, "Err. of pml::constants::sqrt(6.0).");
    static_assert(pml::constants::fabs(pml::constants::sqrt(7.0)  - 2.64575131106459059050) < 1.0e-15, "Err. of pml::constants::sqrt(7.0).");
    static_assert(pml::constants::fabs(pml::constants::sqrt(8.0)  - 2.82842712474619009760) < 1.0e-15, "Err. of pml::constants::sqrt(8.0).");
    static_assert(pml::constants::fabs(pml::constants::sqrt(10.0) - 3.16227766016837933199) < 1.0e-15, "Err. of pml::constants::sqrt(10.0).");
}

TEST(constants, pow)
{
    static_assert((pml::constants::pow( 2.0, 2) ==    4.0), "Err. of pml::constants::pow( 2.0, 2)");
    static_assert((pml::constants::pow(-3.0, 3) ==  -27.0), "Err. of pml::constants::pow(-3.0, 3)");
    static_assert((pml::constants::pow( 4.0, 5) == 1024.0), "Err. of pml::constants::pow( 4.0, 5)");
    static_assert((pml::constants::pow(-3.0, 6) ==  729.0), "Err. of pml::constants::pow(-3.0, 6)");
}