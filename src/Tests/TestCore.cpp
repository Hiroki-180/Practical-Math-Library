#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/CPUDispatcher.h>
#include <PML/Core/aligned_vector.h>

TEST(TestCore, CPUInfo)
{
	ASSERT_NO_THROW(PMLNS::outputCPUInfo(std::cout));
}

TEST(TestCore, aligned_array)
{
	const auto lSize = 100;
	const std::vector<std::size_t> lAligns = { 8, 16, 32, 64, 128 };

	for (const auto& align_i : lAligns)
	{
		const auto lAArray = PMLNS::createAlignedArray<double>(lSize, align_i);

		EXPECT_EQ(0, (uintptr_t)(lAArray.get()) % align_i);
	}
}