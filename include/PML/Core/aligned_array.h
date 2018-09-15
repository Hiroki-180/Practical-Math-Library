#ifndef CORE_ALIGNED_ARRAY_H
#define CORE_ALIGNED_ARRAY_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Memory aligned array pml::aligned::alarray.
*/

#include <memory>
#include <stdexcept>
#include <cstdlib>

#if defined(__cplusplus) && __cplusplus < 201103L

/**
* @brief
* Macro to create the static array that is just aligned on specified byte size boundaries.
* For instance, PML_STATIC_ALLIGN(32) double vec[5]; create 32-byte aligned double array with 5 doubles.
*
* @param[in] n
* Alignment size in byte.
*/
#define PML_STATIC_ALLIGN(n) __declspec(align(n))

#else

/**
* @brief
* Macro to create the static array that is just aligned on specified byte size boundaries.
* For instance, PML_STATIC_ALLIGN(32) double vec[5]; create 32-byte aligned double array with 5 doubles.
*
* @param[in] n
* Alignment size in byte.
*/
#define PML_STATIC_ALLIGN(n) alignas(n)

#endif

#endif