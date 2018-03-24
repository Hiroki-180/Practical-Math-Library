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
