#ifndef CORE_ALIGNED_ARRAY_H
#define CORE_ALIGNED_ARRAY_H

#pragma once

#include <type_traits>
#include <memory>
#include <stdexcept>
#include <vector>

#if defined(__cplusplus) && __cplusplus < 201103L
#define PML_STATIC_ALLIGN(n) __declspec(align(n))
#else
#define PML_STATIC_ALLIGN(n) alignas(n)
#endif

namespace pml {

    namespace detail {

        template<typename T = void*, typename std::enable_if<std::is_pointer<T>::value, std::nullptr_t>::type = nullptr>
        static inline T alignedMalloc(std::size_t inSize, std::size_t inAlignment) noexcept
        {
            return reinterpret_cast<T>(_aligned_malloc(inSize, inAlignment));
        }

        static inline void alignedFree(void* inPtr) noexcept
        {
            _aligned_free(inPtr);
        }

        struct alignedDeleter final
        {
            void operator()(void* p) const noexcept
            {
                alignedFree(p);
            }
        };

    } // detail

    template<typename T>
    using aligned_array = std::unique_ptr<T[], detail::alignedDeleter>;

    template<typename T>
    auto createAlignedArray(std::size_t inSize, std::size_t inAlignment)
    {
        return aligned_array<double>(detail::alignedMalloc<T*>(inSize * sizeof(T), inAlignment));
    }

} // pml

#endif