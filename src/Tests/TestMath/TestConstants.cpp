#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/constants.h>

#include <cmath>

TEST(TestMathConstants, PI)
{
    EXPECT_DOUBLE_EQ(acos(-1.0), pml::constants::PI());
}

TEST(TestMathConstants, PI_Relations)
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

TEST(TestMathConstants, e)
{
    EXPECT_DOUBLE_EQ(1.0, std::log(pml::constants::e()));
}

TEST(TestMathConstants, Sqrts)
{
	EXPECT_DOUBLE_EQ(std::sqrt( 2.0), pml::constants:: SqrtOf2());
	EXPECT_DOUBLE_EQ(std::sqrt( 3.0), pml::constants:: SqrtOf3());
	EXPECT_DOUBLE_EQ(std::sqrt( 5.0), pml::constants:: SqrtOf5());
	EXPECT_DOUBLE_EQ(std::sqrt( 6.0), pml::constants:: SqrtOf6());
	EXPECT_DOUBLE_EQ(std::sqrt( 7.0), pml::constants:: SqrtOf7());
	EXPECT_DOUBLE_EQ(std::sqrt( 8.0), pml::constants:: SqrtOf8());
	EXPECT_DOUBLE_EQ(std::sqrt(10.0), pml::constants::SqrtOf10());
}

TEST(TestMathConstants, GoldenRatio)
{
    EXPECT_DOUBLE_EQ(0.5*(1.0 + pml::constants::SqrtOf5()), pml::constants::GoldenRatio());
}

TEST(TestMathConstants, Apery)
{
    double lRiemannZeta3 = 0.0;
    for (auto i = 20000000; i > 0; --i)
    {
        const double lDen = (static_cast<double>(i)*i*i);
        lRiemannZeta3 += 1.0 / lDen;
    }

    EXPECT_DOUBLE_EQ(lRiemannZeta3, pml::constants::Apery());
}

TEST(TestMathConstants, UniversalParabolicP)
{
    EXPECT_DOUBLE_EQ(
        std::log(1.0 + pml::constants::SqrtOf2()) + pml::constants::SqrtOf2(),
        pml::constants::UniversalParabolicP());
}

TEST(TestMathConstants, RationalNumbers)
{
    EXPECT_DOUBLE_EQ(1.0 / 3.0, pml::constants::Q::_1over3());
    EXPECT_DOUBLE_EQ(1.0 / 6.0, pml::constants::Q::_1over6());
    EXPECT_DOUBLE_EQ(1.0 / 7.0, pml::constants::Q::_1over7());
    EXPECT_DOUBLE_EQ(1.0 / 9.0, pml::constants::Q::_1over9());
}
