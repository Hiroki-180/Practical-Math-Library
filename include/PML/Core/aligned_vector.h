#ifndef CORE_ALIGNED_VECTOR_H
#define CORE_ALIGNED_VECTOR_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Memory aligned customized STL vectors pml::aligned::vector16/32/64.
*/

#include <PML/Core/aligned_array.h>
#include <vector>

namespace pml {
    namespace aligned {

        namespace detail {

            template <class T, int N>
            class aligned_allocator final
            {
            public:

                typedef T value_type;
                typedef T& reference;
                typedef const T& const_reference;
                typedef T* pointer;
                typedef const T* const_pointer;
                typedef size_t size_type;
                typedef ptrdiff_t difference_type;

                template <class U>
                struct rebind
                {
                    typedef aligned_allocator<U, N> other;
                };

                inline aligned_allocator() noexcept {}
                inline aligned_allocator(const aligned_allocator&) noexcept {}

                template <class U>
                inline aligned_allocator(const aligned_allocator<U, N>&) noexcept {}

                inline ~aligned_allocator() noexcept {}

                inline pointer address(reference r) { return &r; }
                inline const_pointer address(const_reference r) const { return &r; }

                pointer allocate(size_type n, typename std::allocator<void>::const_pointer hint = 0) const
                {
                    hint;

                    if (n > this->max_size())
                    {
                        throw std::bad_alloc(); //ToDo.
                    }

                    return pml::aligned::detail::alignedMalloc<pointer>(n * sizeof(T), N);
                }

                void deallocate(pointer p, size_type) const noexcept
                {
                    pml::aligned::detail::alignedFree(p);
                }

                inline void construct(pointer p, const_reference value) { new (p) value_type(value); }
                inline void destroy(pointer p) { p->~value_type(); }

                inline size_type max_size() const noexcept { return size_type(-1) / sizeof(T); }

                inline bool operator==(const aligned_allocator&) { return true; }
                inline bool operator!=(const aligned_allocator& rhs) { return !operator==(rhs); }
            };

        } // detail

        /**
        * @brief
        * Customized STL vector that has proper custom allocator to the 16-byte aligned data.
        * This is proper alignment for SSE/SSE2/SSE3/SSE4.1/SSE4.2.
        */
        template<typename T>
        using alvector16 = std::vector<T, detail::aligned_allocator<T, 16>>;

        /**
        * @brief
        * Customized STL vector that has proper custom allocator to the 32-byte aligned data.
        * This is proper alignment for AVX/AVX2.
        */
        template<typename T>
        using alvector32 = std::vector<T, detail::aligned_allocator<T, 32>>;

        /**
        * @brief
        * Customized STL vector that has proper custom allocator to the 2-byte aligned data.
        * This is proper alignment for AVX512 series.
        */
        template<typename T>
        using alvector64 = std::vector<T, detail::aligned_allocator<T, 64>>;

    } // aligned
} // pml

#endif