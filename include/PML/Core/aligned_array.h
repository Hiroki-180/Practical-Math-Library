#ifndef CORE_ALIGNED_ARRAY_H
#define CORE_ALIGNED_ARRAY_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Memory aligned array creator.
*/

#include <type_traits>
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
#define PML_STATIC_ALLIGN(n) alignas(n)
#endif

namespace pml {

    namespace aligned {

        namespace detail {

            template<typename T = void*, typename std::enable_if<std::is_pointer<T>::value, std::nullptr_t>::type = nullptr>
            static inline T alignedMalloc(std::size_t inSize, std::size_t inAlignment) noexcept
            {
#ifdef _MSC_VER
				return reinterpret_cast<T>(_aligned_malloc(inSize, inAlignment));
#else
				void* lp;
				return reinterpret_cast<T>(posix_memalign(&lp, inAlignment, inSize) == 0 ? lp : nullptr);
#endif
            }

            static inline void alignedFree(void* inPtr) noexcept
            {
#ifdef _MSC_VER
				_aligned_free(inPtr);
#else
				std::free(inPtr);
#endif
			}

            struct alignedDeleter final
            {
                void operator()(void* p) const noexcept
                {
					alignedFree(p);
                }
            };

        } // detail

        /**
        * @brief
        * The unique pointer to the array that has proper custom deleter to the aligned data.
        */
        template<typename T>
        using alarray = std::unique_ptr<T[], detail::alignedDeleter>;

        /**
        * @brief
        * Inline function creating the unique pointer to the array that is just aligned on specified byte size boundaries.
        * The deleter is proper one to the aligned data.
        *
        * @param[in] inSize
        * Size of array.
        *
        * @param[in] inAlignment
        * Alignment size in byte.
        *
        * @return
        * pml::aligned::array<double>
        */
        template<typename T>
        inline alarray<double> createArray(std::size_t inSize, std::size_t inAlignment)
        {
            return alarray<double>(detail::alignedMalloc<T*>(inSize * sizeof(T), inAlignment));
        }

    } // aligned
} // pml

#endif