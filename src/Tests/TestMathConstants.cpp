#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/MathConstants.h>

#include <cmath>

TEST(TestMathConstants, PI)
{
	EXPECT_DOUBLE_EQ(acos(-1.0), PMLMathConstantsNS::PI());
}

TEST(TestMathConstants, PI_Relations)
{
	const auto lAnsPI= std::acos(-1.0);
	EXPECT_DOUBLE_EQ(std::sqrt(lAnsPI), PMLMathConstantsNS::SqrtPI());

	const auto lAnsPIHalf = 0.5*lAnsPI;
    EXPECT_DOUBLE_EQ(lAnsPIHalf, PMLMathConstantsNS::PIHalf());
    EXPECT_DOUBLE_EQ(std::sqrt(lAnsPIHalf), PMLMathConstantsNS::SqrtPIHalf());

	const auto lAnsTwoPI = 2.0*lAnsPI;
    EXPECT_DOUBLE_EQ(lAnsTwoPI, PMLMathConstantsNS::TwoPI());
    EXPECT_DOUBLE_EQ(std::sqrt(lAnsTwoPI), PMLMathConstantsNS::SqrtTwoPI());
    EXPECT_DOUBLE_EQ(1.0 / std::sqrt(lAnsTwoPI), PMLMathConstantsNS::OneOverSqrtTwoPI());

	EXPECT_DOUBLE_EQ(lAnsPI*lAnsPI, PMLMathConstantsNS::PISqrd());
}

TEST(TestMathConstants, e)
{
	EXPECT_DOUBLE_EQ(1.0, std::log(PMLMathConstantsNS::e()));
}

TEST(TestMathConstants, Sqrts)
{
	EXPECT_DOUBLE_EQ(std::sqrt( 2.0), PMLMathConstantsNS:: SqrtOf2());
	EXPECT_DOUBLE_EQ(std::sqrt( 3.0), PMLMathConstantsNS:: SqrtOf3());
	EXPECT_DOUBLE_EQ(std::sqrt( 5.0), PMLMathConstantsNS:: SqrtOf5());
	EXPECT_DOUBLE_EQ(std::sqrt( 6.0), PMLMathConstantsNS:: SqrtOf6());
	EXPECT_DOUBLE_EQ(std::sqrt( 7.0), PMLMathConstantsNS:: SqrtOf7());
	EXPECT_DOUBLE_EQ(std::sqrt( 8.0), PMLMathConstantsNS:: SqrtOf8());
	EXPECT_DOUBLE_EQ(std::sqrt(10.0), PMLMathConstantsNS::SqrtOf10());
}

TEST(TestMathConstants, GoldenRatio)
{
	EXPECT_DOUBLE_EQ(0.5*(1.0 + PMLMathConstantsNS::SqrtOf5()), PMLMathConstantsNS::GoldenRatio());
}

TEST(TestMathConstants, Apery)
{
	double lRiemannZeta3 = 0.0;
	for (auto i = 20000000;i > 0;--i) {
		const double lDen = (static_cast<double>(i)*i*i);
		lRiemannZeta3 += 1.0 / lDen;
	}

	EXPECT_DOUBLE_EQ(lRiemannZeta3, PMLMathConstantsNS::Apery());
}

TEST(TestMathConstants, UniversalParabolicP)
{
	EXPECT_DOUBLE_EQ(
		std::log(1.0 + PMLMathConstantsNS::SqrtOf2()) + PMLMathConstantsNS::SqrtOf2(),
		PMLMathConstantsNS::UniversalParabolicP());
}

TEST(TestMathConstants, RationalNumbers)
{
	EXPECT_DOUBLE_EQ(1.0 / 3.0, PMLMathConstantsNS::Q::_1Over3());
	EXPECT_DOUBLE_EQ(1.0 / 6.0, PMLMathConstantsNS::Q::_1Over6());
	EXPECT_DOUBLE_EQ(1.0 / 7.0, PMLMathConstantsNS::Q::_1Over7());
	EXPECT_DOUBLE_EQ(1.0 / 9.0, PMLMathConstantsNS::Q::_1Over9());
}