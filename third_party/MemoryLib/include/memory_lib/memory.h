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

#include <cstddef>


namespace MemoryLib {

// Some compilers don't have std::align
// Rather than trying to detect which do and manually defining it for those that don't,
// we just use this simple implementation for all
inline void *align(std::size_t alignment, std::size_t size, void *&ptr, std::size_t &space) noexcept {
	const auto intptr = reinterpret_cast<uintptr_t>(ptr);
	#pragma warning(disable : 4146)
	const auto aligned = (intptr - 1u + alignment) & -alignment;
	#pragma warning(default : 4146)
	const auto diff = aligned - intptr;
	if ((size + diff) > space) {
		return nullptr;
	} else {
		space -= diff;
		return ptr = reinterpret_cast<void *>(aligned);
	}
}

template<typename T>
inline void destruct(T *p) {
	p->~T();
}

} // End of namespace MemoryLib
