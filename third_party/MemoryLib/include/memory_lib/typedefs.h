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

#include <cstdint>

typedef unsigned char byte;
typedef std::int8_t  int8;
typedef std::uint8_t  uint8;

typedef std::int16_t int16;
typedef std::uint16_t uint16;

typedef unsigned int uint;
typedef std::int32_t int32;
typedef std::uint32_t uint32;

typedef std::int64_t int64;
typedef std::uint64_t uint64;

typedef wchar_t wchar;
