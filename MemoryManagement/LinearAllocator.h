#pragma once
#include <cstddef>
#include <new>
#include <utility>

class LinearAllocator
{
public:
	LinearAllocator(size_t capacity);

	LinearAllocator(LinearAllocator&& o) noexcept;

	LinearAllocator(const LinearAllocator&) = delete;
	LinearAllocator& operator=(const LinearAllocator&) = delete;

	~LinearAllocator();

	void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));

	void reset() noexcept;

	size_t save() const noexcept;

	void restore(size_t marker);

	size_t used() const noexcept;
	size_t remaining() const noexcept;
	size_t capacity() const noexcept;

	template<typename T, typename... Args>
	T* make(Args&&... args) {           
		void* mem = allocate(sizeof(T), alignof(T));
		return new (mem) T(std::forward<Args>(args)...);
	}

	template<typename T>
	T* make_array(size_t count) {       
		void* mem = allocate(sizeof(T) * count, alignof(T));
		T* arr = static_cast<T*>(mem);
		for (size_t i = 0; i < count; ++i)
			new (arr + i) T();
		return arr;
	}
private:
	std::byte* m_buffer;
	size_t     m_capacity;
	size_t     m_offset;
};

