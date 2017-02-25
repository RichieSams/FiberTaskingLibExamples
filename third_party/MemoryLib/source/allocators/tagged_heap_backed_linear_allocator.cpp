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

#include "memory_lib/tagged_heap_backed_linear_allocator.h"

#include "memory_lib/tagged_heap.h"
#include "memory_lib/memory.h"

#include <cassert>


namespace MemoryLib {

TaggedHeapBackedLinearAllocator::TaggedHeapBackedLinearAllocator(const char *name)
		: m_heap(nullptr),
		  m_id(0u),
		  m_currentPage(nullptr),
		  m_current(nullptr),
		  m_end(nullptr),
		  m_lock(nullptr) {
	#if EASTL_NAME_ENABLED
		m_name = name;
	#endif
}

TaggedHeapBackedLinearAllocator::TaggedHeapBackedLinearAllocator(const TaggedHeapBackedLinearAllocator &other)
		: m_heap(other.m_heap),
		  m_id(other.m_id),
		  m_currentPage(other.m_currentPage),
		  m_current(other.m_current),
		  m_end(other.m_end),
		  m_lock(other.m_lock) {
	#if EASTL_NAME_ENABLED
		m_name = other.m_name;
	#endif
}

TaggedHeapBackedLinearAllocator::TaggedHeapBackedLinearAllocator(const TaggedHeapBackedLinearAllocator &other, const char *name)
		: m_heap(other.m_heap),
		  m_id(other.m_id),
		  m_currentPage(other.m_currentPage),
		  m_current(other.m_current),
		  m_end(other.m_end),
		  m_lock(other.m_lock) {
	#if EASTL_NAME_ENABLED
		m_name = name ? name : EASTL_ALLOCATOR_DEFAULT_NAME;
	#endif
}

TaggedHeapBackedLinearAllocator &TaggedHeapBackedLinearAllocator::operator=(const TaggedHeapBackedLinearAllocator &alloc) {
	#if EASTL_NAME_ENABLED
		m_name = alloc.m_name;
	#endif

	m_heap = alloc.m_heap;
	m_id = alloc.m_id;

	m_currentPage = alloc.m_currentPage;

	m_current = alloc.m_current;
	m_end = alloc.m_end;

	m_lock = alloc.m_lock;

	return *this;
}


void TaggedHeapBackedLinearAllocator::init(TaggedHeap *heap, uint64 id) {
	m_heap = heap;
	m_id = id;

	m_currentPage = new MemoryPage *();

	m_current = new byte *();
	m_end = new byte *();

	m_lock = new std::mutex();

	AskForNewPageFromHeap();
}

void TaggedHeapBackedLinearAllocator::retarget(uint64 newId) {
	m_id = newId;

	AskForNewPageFromHeap();
}

void TaggedHeapBackedLinearAllocator::destroy() {
	delete m_currentPage;
	delete m_current;
	delete m_end;
	delete m_lock;
}


void *TaggedHeapBackedLinearAllocator::allocate(std::size_t n, int flags) {
	std::lock_guard<std::mutex> guard(*m_lock);
	
	assert(m_currentPage);
	if (n > (*m_currentPage)->PageSize) {
		return nullptr;
	}

	if (*m_current + n >= *m_end) {
		AskForNewPageFromHeap();
	}

	void *userPtr = *m_current;
	*m_current += n;

	return userPtr;
}

void *TaggedHeapBackedLinearAllocator::allocate(std::size_t n, std::size_t alignment, std::size_t offset, int flags) {
	std::lock_guard<std::mutex> guard(*m_lock);
	
	assert(m_currentPage);
	if (n > (*m_currentPage)->PageSize) {
		return nullptr;
	}

	if (*m_current + n >= *m_end) {
		AskForNewPageFromHeap();
	}

	std::size_t bufferSize = *m_end - *m_current;
	void *start = *m_current;
	void *userPtr;
	while ((userPtr = MemoryLib::align(alignment, n, start, bufferSize)) == nullptr) {
		AskForNewPageFromHeap();

		// Recalculate bounds for the align check
		bufferSize = *m_end - *m_current;
		start = *m_current;
	}

	*m_current += ((byte *)userPtr + n) - (byte *)start;

	return userPtr;
}

// EASTL expects us to define these operators (allocator.h L103)
bool operator==(const TaggedHeapBackedLinearAllocator &a, const TaggedHeapBackedLinearAllocator &b) {
	return a.m_heap == b.m_heap &&
	       a.m_id == b.m_id &&
	       a.m_currentPage == b.m_currentPage &&
	       a.m_current == b.m_current &&
	       a.m_end == b.m_end &&
	       a.m_lock == b.m_lock;
}
bool operator!=(const TaggedHeapBackedLinearAllocator &a, const TaggedHeapBackedLinearAllocator &b) {
	return a.m_heap != b.m_heap ||
	       a.m_id != b.m_id ||
	       a.m_currentPage != b.m_currentPage ||
	       a.m_current != b.m_current ||
	       a.m_end != b.m_end ||
	       a.m_lock != b.m_lock;
}

} // End of namespace MemoryLib
