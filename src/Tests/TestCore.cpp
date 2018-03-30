#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/CPUDispatcher.h>
#include <PML/Core/aligned_vector.h>

TEST(TestCore, CPUInfo)
{
    EXPECT_NO_THROW(pml::CPUDispatcher::outputCPUInfo(std::cout));
}

TEST(TestCore, aligned_array)
{
    const auto lSize = 100;
    const std::vector<std::size_t> lAligns = { 2, 4, 8, 16, 32, 64, 128 };

    for (const auto& align_i : lAligns)
    {
        const auto lAArray = pml::createAlignedArray<double>(lSize, align_i);

        EXPECT_EQ(0, (uintptr_t)(lAArray.get()) % align_i);
    }
}

TEST(TestCore, align2_vector)
{
    const std::size_t lSize = 100;
    pml::align2_vector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(&(lAVector.data()[0])) % 2);
}

TEST(TestCore, align4_vector)
{
    const std::size_t lSize = 100;
    pml::align4_vector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(&(lAVector.data()[0])) % 4);
}

TEST(TestCore, align8_vector)
{
    const std::size_t lSize = 100;
    pml::align8_vector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(&(lAVector.data()[0])) % 8);
}

TEST(TestCore, align16_vector)
{
    const std::size_t lSize = 100;
    pml::align16_vector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(&(lAVector.data()[0])) % 16);
}

TEST(TestCore, align32_vector)
{
    const std::size_t lSize = 100;
    pml::align32_vector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(lAVector.data()) % 32);
}

TEST(TestCore, align64_vector)
{
    const std::size_t lSize = 100;
    pml::align64_vector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(lAVector.data()) % 64);
}