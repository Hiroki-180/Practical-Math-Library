#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Math/derivative.h>
#include <cmath>
#include <functional>

TEST(derivative_1st_fwd, sin)
{
    const auto lAns = std::cos(1.0);

    EXPECT_NEAR(
        lAns,
        pml::derivative_1st_fwd_1stAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-6);

    EXPECT_NEAR(
        lAns,
        pml::derivative_1st_fwd_2ndAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-9);

    EXPECT_NEAR(
        lAns,
        pml::derivative_1st_fwd_3rdAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-9);

    EXPECT_NEAR(
        lAns,
        pml::derivative_1st_fwd_4thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-13);

    EXPECT_NEAR(
        lAns,
        pml::derivative_1st_fwd_5thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-9);

    EXPECT_NEAR(
        lAns,
        pml::derivative_1st_fwd_6thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-6),
        std::fabs(lAns)*1e-8);
}

TEST(derivative_2nd_fwd, sin)
{
    const auto lAns = -std::sin(1.0);

    EXPECT_NEAR(
        lAns,
        pml::derivative_2nd_fwd_1stAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-5),
        std::fabs(lAns) * 1e-5);

    EXPECT_NEAR(
        lAns,
        pml::derivative_2nd_fwd_2ndAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-5),
        std::fabs(lAns) * 1e-5);

    EXPECT_NEAR(
        lAns,
        pml::derivative_2nd_fwd_3rdAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-5),
        std::fabs(lAns) * 1e-5);

    EXPECT_NEAR(
        lAns,
        pml::derivative_2nd_fwd_4thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-5),
        std::fabs(lAns) * 1e-4);

    EXPECT_NEAR(
        lAns,
        pml::derivative_2nd_fwd_5thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-5),
        std::fabs(lAns) * 1e-4);

    EXPECT_NEAR(
        lAns,
        pml::derivative_2nd_fwd_6thAcc(std::bind((double(*)(double))&std::sin, std::placeholders::_1), 1.0, 1e-5),
        std::fabs(lAns) * 1e-4);
}