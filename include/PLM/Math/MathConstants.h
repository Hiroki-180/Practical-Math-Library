#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#pragma once

#include <PLM/Core/NamespaceMacros.h>

PML_MATH_NS_BEGIN namespace constants
{
	inline double PI() { return 3.141592653589793; }
	inline double TwoPI() { return 6.283185307179586; }
	inline double SqrtTwoPI() { return 2.506628274631000; }
	inline double OneOverSqrtTwoPI() { return 0.3989422804014327; }
}
PML_MATH_NS_END

#endif