#pragma once

template<size_t Type>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, Type);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};