#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

#include <PML/Core/NamespaceMacros.h>

PML_MATH_NS_BEGIN namespace constants
{
	inline double PI() { return 3.141592653589793; }
	inline double TwoPI() { return 6.283185307179586; }
	inline double SqrtTwoPI() { return 2.506628274631000; }
	inline double OneOverSqrtTwoPI() { return 0.3989422804014327; }

	inline double e() { return 2.718281828459045; }
	inline double SqrtOf2() { return 1.414213562373095; }
	inline double SqrtOf3() { return 1.732050807568877; }
	inline double SqrtOf5() { return 2.236067977499789; }

	inline double GoldenRatio(){ return 1.618033988749894; }
	inline double Apery() { return 1.202056903159594; }
	inline double UniversalParabolicP() { return 2.295587149392638; }
}
PML_MATH_NS_END

#endif