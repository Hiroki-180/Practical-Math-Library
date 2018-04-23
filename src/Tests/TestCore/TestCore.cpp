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
        const auto lAArray = pml::aligned::createArray<double>(lSize, align_i);

        EXPECT_EQ(0, (uintptr_t)(lAArray.get()) % align_i);
    }
}

TEST(TestCore, 16_Byte_alignment_vector)
{
    const std::size_t lSize = 100;
    pml::aligned::vector16<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(&(lAVector.data()[0])) % 16);
}

TEST(TestCore, 32_Byte_alignment_vector)
{
    const std::size_t lSize = 100;
    pml::aligned::vector32<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(lAVector.data()) % 32);
}

TEST(TestCore, 64_Byte_alignment_vector)
{
    const std::size_t lSize = 100;
    pml::aligned::vector64<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());
    EXPECT_EQ(0, (uintptr_t)(lAVector.data()) % 64);
}
