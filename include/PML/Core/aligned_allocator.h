#ifndef CORE_ALIGNED_ALLOCATOR_H
#define CORE_ALIGNED_ALLOCATOR_H

/**
* @file
* public header provided by PML.
*
* @brief
* Automatically memory aligned customized STL vector pml::aligned::alvector.
*/

#include <PML/Core/CPUDispatcher.h>
#include <type_traits>
#include <cstdlib>
#include <stdexcept>
#include <vector>

namespace pml {
    namespace aligned {

        namespace detail {

            template
                <typename T = void*,
                 std::enable_if_t<std::is_pointer<T>::value, std::nullptr_t> = nullptr>
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

                using value_type      =       T ;
                using reference       =       T&;
                using const_reference = const T&;
                using pointer         =       T*;
                using const_pointer   = const T*;

                using size_type       = std::size_t;
                using difference_type = std::ptrdiff_t;

                template <class U>
                struct rebind final
                {
                    using other = aligned_allocator<U>;
                };

                inline aligned_allocator() noexcept
                {}
                
                inline aligned_allocator(const aligned_allocator&) noexcept
                {}

                template <class U>
                inline aligned_allocator(const aligned_allocator<U>&) noexcept
                {}

                inline ~aligned_allocator() noexcept
                {}

                inline pointer address(reference r)
                {
                    return &r;
                }
                
                inline const_pointer address(const_reference r) const
                {
                    return &r;
                }

                pointer allocate(size_type n) const
                {
                    if (n > this->max_size()){
                        throw std::bad_alloc();
                    }

                    return pml::aligned::detail::alignedMalloc<pointer>(n*sizeof(T), CPUDispatcher::getOptimalAlignment());
                }

                void deallocate(pointer p, size_type) const noexcept
                {
                    pml::aligned::detail::alignedFree(p);
                }

                inline void construct(pointer p, const_reference value)
                {
                    new (p) value_type(value);
                }

                inline void destroy(pointer p)
                {
                    p->~value_type();
                }

                inline size_type max_size() const noexcept
                { 
                    return size_type(-1) / sizeof(T);
                }

                inline bool operator==(const aligned_allocator&)
                {
                    return true;
                }
                
                inline bool operator!=(const aligned_allocator& rhs)
                {
                    return !operator==(rhs);
                }
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