/* FiberTaskingLibExamples
*
* This is a set of examples to accompany the FiberTaskingLib
* https://github.com/RichieSams/FiberTaskingLib
*
* FiberTaskingLibExamples is the legal property of Adrian Astley
* Copyright Adrian Astley 2016
*/

#pragma once

#include "utils/spsc_queue.h"


struct GLFWwindow;

struct GameSyncState {
	GameSyncState()
		: SceneQueue(4),
		ShouldQuit(false) {}

	SPSCQueue SceneQueue;
	std::atomic_bool ShouldQuit;
};

struct ThreadArgs {
	GLFWwindow *Window;

	GameSyncState *SyncState;
};