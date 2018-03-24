#include "stdafx.h"
#include <gtest/gtest.h>

#include <PML/Math/MathConstants.h>

TEST(TestMathConstants, Relations_PI)
{
	const auto lAnsTwoPI = 2.0*PMLMathConstantsNS::PI();

	ASSERT_DOUBLE_EQ(lAnsTwoPI, PMLMathConstantsNS::TwoPI());
	ASSERT_DOUBLE_EQ(std::sqrt(lAnsTwoPI), PMLMathConstantsNS::SqrtTwoPI());
	ASSERT_DOUBLE_EQ(1.0 / std::sqrt(lAnsTwoPI), PMLMathConstantsNS::OneOverSqrtTwoPI());
}
