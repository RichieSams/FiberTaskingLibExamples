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

// Determine the OS
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	#define MEMLIB_OS_WINDOWS
#elif defined(__APPLE__)
    #include "TargetConditionals.h"

    #if defined(TARGET_OS_MAC)
        #define MEMLIB_OS_MAC
    #elif defined(TARGET_OS_IPHONE)
        #define MEMLIB_OS_iOS
    #else
		#error Unknown Apple platform
	#endif
#elif defined(__linux__)
	#define MEMLIB_OS_LINUX
#endif
