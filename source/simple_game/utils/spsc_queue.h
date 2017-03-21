/* FiberTaskingLibExamples
*
* This is a set of examples to accompany the FiberTaskingLib
* https://github.com/RichieSams/FiberTaskingLib
*
* FiberTaskingLibExamples is the legal property of Adrian Astley
* Copyright Adrian Astley 2016
*/

#pragma once

#include "utils/typedefs.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <cassert>


typedef uint64 T;

class SPSCQueue {
public:
	SPSCQueue(std::size_t size)
			: m_top(1),
			  m_bottom(1), 
			  m_data(new T[size]), 
			  m_size(size) {
		assert(!(size == 0) && !(size & (size - 1)) && "n must be a power of 2");
	}
	~SPSCQueue() {
		delete[] m_data;
	}


private:
	std::atomic_uint m_top;
	std::atomic_uint m_bottom;

	std::mutex m_topMutex;
	std::condition_variable m_topCV;
	std::mutex m_bottomMutex;
	std::condition_variable m_bottomCV;

	T *m_data;
	std::size_t m_size;

public:
	void Push(T value) {
		T b = m_bottom.load(std::memory_order_relaxed);
		T t = m_top.load(std::memory_order_acquire);

		if (b - t > m_size - 1) {
			// Queue is full
			// Wait until the consumer consumes an item
			std::unique_lock<std::mutex> topLock(m_topMutex);
			while (b - m_top.load(std::memory_order_acquire) > m_size - 1) {
				m_topCV.wait(topLock);
			}
		}
		m_data[b & (m_size - 1)] = value;
		std::atomic_thread_fence(std::memory_order_release);

		m_bottom.fetch_add(1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
	
		m_bottomCV.notify_one();
	}

	T Pop() {
		T t = m_top.load(std::memory_order_relaxed);
		T b = m_bottom.load(std::memory_order_acquire);

		if (t >= b) {
			// Queue is empty
			// Wait until the producer creates something to consume
			std::unique_lock<std::mutex> bottomLock(m_bottomMutex);
			while (t >= m_bottom.load(std::memory_order_acquire)) {
				m_bottomCV.wait(bottomLock);
			}
		}

		// Non-empty queue
		T value = m_data[t & (m_size - 1)];
		std::atomic_thread_fence(std::memory_order_release);

		m_top.fetch_add(1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);

		m_topCV.notify_one();

		return value;
	}
};
