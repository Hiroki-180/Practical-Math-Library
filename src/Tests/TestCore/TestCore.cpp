#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/CPUDispatcher.h>

TEST(TestCore, CPUInfo)
{
    EXPECT_NO_THROW(pml::CPUDispatcher::outputCPUInfo(std::cout));
}