#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/CPUDispatcher.h>
#include <PML/Core/aligned_vector.h>

TEST(TestCore, CPUInfo)
{
    EXPECT_NO_THROW(pml::CPUDispatcher::outputCPUInfo(std::cout));
}

TEST(TestCore, aligned_vector)
{
    const std::size_t lSize = 100;
    pml::aligned::alvector<double> lAVector(lSize);

    EXPECT_EQ(lSize, lAVector.size());

    std::size_t align = pml::CPUDispatcher::isAVX512F() ? 64 :
                        pml::CPUDispatcher::isAVX2()    ? 32 :
                        pml::CPUDispatcher::isAVX()     ? 32 :
                                                          16;

    EXPECT_EQ(0U, (uintptr_t)(&(lAVector.data()[0])) % align);
}