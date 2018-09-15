#ifndef CORE_ALIGNED_VECTOR_H
#define CORE_ALIGNED_VECTOR_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Automatically memory aligned customized STL vector pml::aligned::alvector.
*/

#include <PML/Core/CPUDispatcher.h>
#include <PML/Core/aligned_array.h>
#include <type_traits>
#include <vector>

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

            template <class T>
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
                    typedef aligned_allocator<U> other;
                };

                inline aligned_allocator() noexcept {}
                inline aligned_allocator(const aligned_allocator&) noexcept {}

                template <class U>
                inline aligned_allocator(const aligned_allocator<U>&) noexcept {}

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

                    const std::size_t align = CPUDispatcher::isAVX512F() ? 64 :
                                              CPUDispatcher::isAVX2()    ? 32 :
                                              CPUDispatcher::isAVX()     ? 32 :
                                                                           16 ;

                    return pml::aligned::detail::alignedMalloc<pointer>(n * sizeof(T), align);
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
        * Customized STL vector that has runtime-optimal aligment allocator.
        */
        template<typename T>
        using alvector = std::vector<T, detail::aligned_allocator<T>>;

    } // aligned
} // pml

#endif