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

#include "memory_lib/tagged_heap.h"


namespace MemoryLib {

TaggedHeap::TaggedHeap(std::size_t pageSize)
	: PageSize(pageSize) {
}

TaggedHeap::~TaggedHeap() {
	for (auto &kvp : m_usedMemory) {
		MemoryNode *currentNode = kvp.second;
		MemoryNode *nodeToDelete;

		do {
			nodeToDelete = currentNode;
			currentNode = currentNode->NextNode;

			delete nodeToDelete;
		} while (currentNode != nullptr);
	}
	while (!m_freeMemory.empty()) {
		delete m_freeMemory.front();
		m_freeMemory.pop();
	}
}

MemoryPage *TaggedHeap::GetNextFreePage(uint64 id) {
	std::lock_guard<std::mutex> guard(m_memoryLock);

	MemoryNode *newNode;
	if (m_freeMemory.empty()) {
		newNode = new MemoryNode(PageSize);
	} else {
		newNode = m_freeMemory.front();
		m_freeMemory.pop();
	}
	
	auto iter = m_usedMemory.find(id);
	if (iter != m_usedMemory.end()) {
		newNode->NextNode = iter->second;
	} else {
		newNode->NextNode = nullptr;
	}

	m_usedMemory[id] = newNode;

	return &newNode->Page;
}

void TaggedHeap::FreeAllPagesWithId(uint64 id) {
	std::lock_guard<std::mutex> guard(m_memoryLock);

	auto iter = m_usedMemory.find(id);
	if (iter == m_usedMemory.end()) {
		return;
	}

	MemoryNode *head = iter->second;
	do {
		m_freeMemory.push(head);
		head = head->NextNode;
	} while (head != nullptr);

	m_usedMemory.erase(id);
}

} // End of namespace MemoryLib
