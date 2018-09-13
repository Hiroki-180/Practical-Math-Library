#include "stdafx.h"

#include <gtest/gtest.h>
#include <PML/Core/CPUDispatcher.h>
#include <PML/Core/aligned_vector.h>

namespace {

	template<typename T>
	void test_alvector(unsigned int inAlignmentByte)
	{
		const std::size_t lSize = 100;
		T lAVector(lSize);

		EXPECT_EQ(lSize, lAVector.size());
		EXPECT_EQ(0U, (uintptr_t)(&(lAVector.data()[0])) % inAlignmentByte);
	}
}

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

        EXPECT_EQ(0U, (uintptr_t)(lAArray.get()) % align_i);
    }
}

TEST(TestCore, aligned_vector)
{
	test_alvector<pml::aligned::alvector16<double>>(16);
	test_alvector<pml::aligned::alvector32<double>>(32);
	test_alvector<pml::aligned::alvector64<double>>(64);
}