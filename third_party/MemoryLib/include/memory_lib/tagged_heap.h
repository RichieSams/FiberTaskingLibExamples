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

#include <unordered_map>
#include <queue>
#include <mutex>


namespace MemoryLib {

struct MemoryPage {
	MemoryPage(std::size_t pageSize)
		: PageSize(pageSize),
		  Data(::operator new(pageSize)) {}
	~MemoryPage() {
		::operator delete(Data);
	}

	const std::size_t PageSize;
	void *Data;
};

class TaggedHeap {
public:
	TaggedHeap(std::size_t pageSize);
	~TaggedHeap();

public:
	const std::size_t PageSize;

private:
	struct MemoryNode {
		MemoryNode(std::size_t pageSize)
			: Page(pageSize),
			  NextNode(nullptr) {
		}

		MemoryPage Page;
		MemoryNode *NextNode;
	};

	std::unordered_map<uint64, MemoryNode *> m_usedMemory;
	std::queue<MemoryNode *> m_freeMemory;

	std::mutex m_memoryLock;

public:
	MemoryPage *GetNextFreePage(uint64 id);
	void FreeAllPagesWithId(uint64 id);
};

} // End of namespace MemoryLib
