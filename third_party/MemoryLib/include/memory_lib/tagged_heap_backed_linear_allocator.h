/* MemoryLib - A collection of C++ memory allocators and memory functions
 *
 * This library was created as a proof of concept of the ideas presented by
 * Christian Gyrling in his 2015 GDC Talk 'Parallelizing the Naughty Dog Engine Using Fibers'
 *
 * http://gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine
 *
 * MemoryLib is the legal property of Adrian Astley
 * Copyright Adrian Astley 2017
 */


#pragma once

#include "memory_lib/typedefs.h"

#include "memory_lib/tagged_heap.h"

#include <memory>


namespace MemoryLib {

struct MemoryPage;

#define TAGGED_HEAP_LINEAR_ALLOCATOR_DEFAULT_NAME "TaggedHeapLinearAllocator"

class TaggedHeapBackedLinearAllocator {
public:
	// Constructors
	TaggedHeapBackedLinearAllocator(const char *name = TAGGED_HEAP_LINEAR_ALLOCATOR_DEFAULT_NAME);
	TaggedHeapBackedLinearAllocator(const TaggedHeapBackedLinearAllocator &other);
	TaggedHeapBackedLinearAllocator(const TaggedHeapBackedLinearAllocator &other, const char *name);
	~TaggedHeapBackedLinearAllocator();

	// Assignment
	TaggedHeapBackedLinearAllocator &operator=(const TaggedHeapBackedLinearAllocator &alloc);

private:
	#if EASTL_NAME_ENABLED
		const char *m_name;
	#endif

	TaggedHeap *m_heap;
	uint64 m_id; // Effectively const

	MemoryPage **m_currentPage;

	byte **m_current;
	byte **m_end;

	std::mutex *m_lock;

public:
	void init(TaggedHeap *heap, uint64 id);
	void retarget(uint64 id);
	void destroy();

	// Allocation & Deallocation
	void *allocate(std::size_t n, int flags = 0);
	void *allocate(std::size_t n, std::size_t alignment, std::size_t offset, int flags = 0);

	// Deallocate the memory. NOTE: This will *not* call the destructor for the underlying data type
	inline void deallocate(void *p, std::size_t n) {
		// NoOp
		// In order to free the memory, the user needs to flush the heap
	}

	// Name info
	inline const char *get_name() const {
		#if EASTL_NAME_ENABLED
			return m_name;
		#else
			return TAGGED_HEAP_LINEAR_ALLOCATOR_DEFAULT_NAME;
		#endif
	}
	inline void set_name(const char *name) {
		#if EASTL_NAME_ENABLED
			m_name = name;
		#endif
	}

	friend bool operator==(const TaggedHeapBackedLinearAllocator &a, const TaggedHeapBackedLinearAllocator &b);
	friend bool operator!=(const TaggedHeapBackedLinearAllocator &a, const TaggedHeapBackedLinearAllocator &b);

private:
	inline void AskForNewPageFromHeap() const {
		*m_currentPage = m_heap->GetNextFreePage(m_id);
		*m_current = reinterpret_cast<byte *>((*m_currentPage)->Data);
		*m_end = *m_current + (*m_currentPage)->PageSize;
	}
};

} // End of namespace MemoryLib
