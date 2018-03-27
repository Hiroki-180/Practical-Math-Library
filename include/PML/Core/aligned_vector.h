#ifndef CORE_ALIGNED_VECTOR_H
#define CORE_ALIGNED_VECTOR_H

#pragma once

#include <PML/Core/aligned_array.h>

PML_NS_BEGIN

namespace detail {

	template<typename T, std::size_t N>
	class alignedAllocator final : public std::allocator<T>
	{
	public:
		using size_type     = typename std::allocator<T>::size_type;
		using pointer       = typename std::allocator<T>::pointer;
		using const_pointer = typename std::allocator<T>::const_pointer;

		pointer allocate(size_type inSize, const_pointer = nullptr) const
		{
			if (inSize > this->max_size())
			{
				throw std::bad_alloc("error."); //ToDo.
			}

			return detail::alignedMalloc<pointer>(inSize * sizeof(T), N);
		}

		void deallocate(pointer inPtr, size_type) const noexcept
		{
			detail::alignedFree(inPtr);
		}
	};
}

template<typename T>
using align2_vector = std::vector<T, detail::alignedAllocator<T, 2>>;

template<typename T>
using align4_vector = std::vector<T, detail::alignedAllocator<T, 4>>;

template<typename T>
using align8_vector = std::vector<T, detail::alignedAllocator<T, 8>>;

template<typename T>
using align16_vector = std::vector<T, detail::alignedAllocator<T, 16>>;

template<typename T>
using align32_vector = std::vector<T, detail::alignedAllocator<T, 32>>;

template<typename T>
using align64_vector = std::vector<T, detail::alignedAllocator<T, 64>>;

template<typename T>
using align128_vector = std::vector<T, detail::alignedAllocator<T, 128>>;

PML_NS_END

#endif