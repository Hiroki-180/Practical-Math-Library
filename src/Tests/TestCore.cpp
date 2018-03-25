#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/CPUDetector.h>

TEST(TestCore, CPUInfo)
{
	ASSERT_NO_THROW(PMLNS::outputCPUInfo(std::cout));
}