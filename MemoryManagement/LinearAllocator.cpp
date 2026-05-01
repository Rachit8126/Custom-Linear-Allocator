#include "LinearAllocator.h"

#include <cassert>
#include <stdexcept>
#include <cstdint>

LinearAllocator::LinearAllocator(size_t capacity) : m_capacity(capacity), m_offset(0)
{
	m_buffer = static_cast<std::byte*>(::operator new(m_capacity));
}

LinearAllocator::LinearAllocator(LinearAllocator&& o) noexcept : m_buffer(o.m_buffer), m_capacity(o.m_capacity), m_offset(o.m_offset)
{
	o.m_buffer = nullptr;
	o.m_capacity = o.m_offset = 0;
}

LinearAllocator::~LinearAllocator()
{
	::operator delete(m_buffer);
}

void* LinearAllocator::allocate(size_t size, size_t alignment)
{
	assert((alignment & (alignment - 1)) == 0 && "alignment must be power of two");

	uintptr_t current = reinterpret_cast<uintptr_t>(m_buffer) + m_offset;

	uintptr_t aligned = (current + alignment - 1) & ~(alignment - 1);

	size_t padding = aligned - current;

	if (m_offset + padding + size > m_capacity)
	{
		throw std::bad_alloc();
	}

	m_offset += padding + size;

	return reinterpret_cast<void*>(aligned);
}

void LinearAllocator::reset() noexcept
{
	m_offset = 0;
}

size_t LinearAllocator::save() const noexcept
{
	return m_offset;
}

void LinearAllocator::restore(size_t marker)
{
	assert(marker <= m_offset);
	m_offset = marker;
}

size_t LinearAllocator::used()      const noexcept { return m_offset; }
size_t LinearAllocator::remaining() const noexcept { return m_capacity - m_offset; }
size_t LinearAllocator::capacity()  const noexcept { return m_capacity; }
