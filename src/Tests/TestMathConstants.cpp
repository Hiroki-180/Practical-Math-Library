#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/MathConstants.h>

#include <cmath>

TEST(TestMathConstants, Value_PI)
{
	ASSERT_DOUBLE_EQ(acos(-1.0), PMLMathConstantsNS::PI());
}

TEST(TestMathConstants, Relations_PI)
{
	const auto lAnsTwoPI = 2.0*std::acos(-1.0);

	ASSERT_DOUBLE_EQ(lAnsTwoPI, PMLMathConstantsNS::TwoPI());
	ASSERT_DOUBLE_EQ(std::sqrt(lAnsTwoPI), PMLMathConstantsNS::SqrtTwoPI());
	ASSERT_DOUBLE_EQ(1.0 / std::sqrt(lAnsTwoPI), PMLMathConstantsNS::OneOverSqrtTwoPI());
}

TEST(TestMathConstants, Value_e)
{
	ASSERT_DOUBLE_EQ(1.0, std::log(PMLMathConstantsNS::e()));
}

TEST(TestMathConstants, Value_Sqrts)
{
	ASSERT_DOUBLE_EQ(std::sqrt(2.0), PMLMathConstantsNS::SqrtOf2());
	ASSERT_DOUBLE_EQ(std::sqrt(3.0), PMLMathConstantsNS::SqrtOf3());
	ASSERT_DOUBLE_EQ(std::sqrt(5.0), PMLMathConstantsNS::SqrtOf5());
}

TEST(TestMathConstants, Value_GoldenRatio)
{
	ASSERT_DOUBLE_EQ(0.5*(1.0 + PMLMathConstantsNS::SqrtOf5()), PMLMathConstantsNS::GoldenRatio());
}

TEST(TestMathConstants, Value_Apery)
{
	double lRiemannZeta3 = 0.0;
	for (auto i = 20000000;i > 0;--i) {
		const double lDen = (static_cast<double>(i)*i*i);
		lRiemannZeta3 += 1.0 / lDen;
	}

	ASSERT_DOUBLE_EQ(lRiemannZeta3, PMLMathConstantsNS::Apery());
}

TEST(TestMathConstants, Value_P)
{
	ASSERT_DOUBLE_EQ(
		std::log(1.0+PMLMathConstantsNS::SqrtOf2())+PMLMathConstantsNS::SqrtOf2(),
		PMLMathConstantsNS::UniversalParabolicP());
}