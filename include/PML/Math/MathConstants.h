#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

#include <PML/Core/NamespaceMacros.h>

PML_MATH_NS_BEGIN namespace constants
{
	inline double PI() noexcept { return 3.141592653589793; }
	inline double TwoPI() noexcept { return 6.283185307179586; }
	inline double SqrtTwoPI() noexcept { return 2.506628274631000; }
	inline double OneOverSqrtTwoPI() noexcept { return 0.3989422804014327; }

	inline double e() noexcept { return 2.718281828459045; }
	inline double SqrtOf2() noexcept { return 1.414213562373095; }
	inline double SqrtOf3() noexcept { return 1.732050807568877; }
	inline double SqrtOf5() noexcept { return 2.236067977499789; }

	inline double GoldenRatio() noexcept { return 1.618033988749894; }
	inline double Apery() noexcept { return 1.202056903159594; }
	inline double UniversalParabolicP() noexcept { return 2.295587149392638; }
}
PML_MATH_NS_END

#endif