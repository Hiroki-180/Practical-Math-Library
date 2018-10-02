#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/derivative.h>
#include <cmath>
#include <functional>

TEST(derivative, sin)
{
    const auto lAns = std::cos(1.0);

    EXPECT_NEAR(
        lAns,
        pml::fwd1stDerivative_1stAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-6);

    EXPECT_NEAR(
        lAns,
        pml::fwd1stDerivative_2ndAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-9);

    EXPECT_NEAR(
        lAns,
        pml::fwd1stDerivative_3rdAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-9);

    EXPECT_NEAR(
        lAns,
        pml::fwd1stDerivative_4thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-13);

    EXPECT_NEAR(
        lAns,
        pml::fwd1stDerivative_5thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-9);

    EXPECT_NEAR(
        lAns,
        pml::fwd1stDerivative_6thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-8);
}