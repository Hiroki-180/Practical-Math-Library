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
#include <exception>
#include <stdexcept>
#include <vector>

namespace pml {
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
    }

    template <class T>
    class aligned_allocator final
    {
    public:
        aligned_allocator()  = default;
        ~aligned_allocator() = default;
        aligned_allocator(const aligned_allocator&)            = default;
        aligned_allocator(aligned_allocator&&)                 = default;
        aligned_allocator& operator=(const aligned_allocator&) = default;
        aligned_allocator& operator=(aligned_allocator&&)      = default;

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

        inline pointer address(reference inR)
        {
            return &inR;
        }

        inline const_pointer address(const_reference inR) const
        {
            return &inR;
        }

        pointer allocate(size_type inN) const
        {
            if (constexpr auto max = std::numeric_limits<T>::max()/sizeof(T);  inN > max) {
                std::throw_with_nested(std::bad_alloc());
            }

            return detail::alignedMalloc<pointer>(inN*sizeof(T), CPUDispatcher::getOptimalAlignment());
        }

        void deallocate(pointer inP, size_type) const noexcept
        {
            detail::alignedFree(inP);
        }

        inline void construct(pointer inO, const_reference inValue)
        {
            new (inP) value_type(inValue);
        }

        inline void destroy(pointer inP)
        {
            inP->~value_type();
        }

        inline constexpr size_type max_size() noexcept
        {
            return std::numeric_limits<T>::max()/sizeof(T);
        }

        inline constexpr bool operator==(const aligned_allocator&)
        {
            return true;
        }

        inline bool operator!=(const aligned_allocator& inRhs)
        {
            return !operator==(inRhs);
        }
    };
} // pml

#endif