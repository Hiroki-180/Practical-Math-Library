#ifndef CORE_NAMESPACEMACROS_H
#define CORE_NAMESPACEMACROS_H

#pragma once

#define PML_NS_BEGIN namespace pml {
#define PML_NS_END   }
PML_NS_BEGIN PML_NS_END
namespace PMLNS = pml;

#define PML_MATH_NS_BEGIN PML_NS_BEGIN namespace math {
#define PML_MATH_NS_END   } PML_NS_END
PML_MATH_NS_BEGIN PML_MATH_NS_END
namespace PMLMathNS = pml::math;

#define PML_MATH_CONSTANTS_NS_BEGIN PML_MATH_NS_BEGIN namespace constants {
#define PML_MATH_CONSTANTS_NS_END   } PML_MATH_NS_END
PML_MATH_CONSTANTS_NS_BEGIN PML_MATH_CONSTANTS_NS_END
namespace PMLMathConstantsNS = pml::math::constants;

#define PML_MATH_NUMERIC_SIMD_NS_BEGIN PML_MATH_NS_BEGIN namespace simd {
#define PML_MATH_NUMERIC_SIMD_NS_END   } PML_MATH_NS_END
PML_MATH_NUMERIC_SIMD_NS_BEGIN PML_MATH_NUMERIC_SIMD_NS_END
namespace PMLMathNumericSIMDNS = pml::math::simd;

#endif