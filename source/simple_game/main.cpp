/* FiberTaskingLibExamples
 *
 * This is a set of examples to accompany the FiberTaskingLib
 * https://github.com/RichieSams/FiberTaskingLib
 *
 * FiberTaskingLibExamples is the legal property of Adrian Astley
 * Copyright Adrian Astley 2016
 */


#include "renderer/renderer.h"

#include "logic/logic.h"
#include "logic/game_sync_state.h"

#include <remotery/remotery.h>

#include <GLFW/glfw3.h>
#include <fiber_tasking_lib/task_scheduler.h>


int main(const int argc, const char **argv) {
	GLFWwindow *window = CreateGLFWWindow(1280, 720, "FiberTaskingLib Examples - Simple Game");
	GameSyncState *syncState = new GameSyncState();
	Remotery *rmt;
	rmt_CreateGlobalInstance(&rmt);
	
	// Start the render thread
	ThreadArgs *renderArgs = new ThreadArgs;
	renderArgs->Window = window;
	renderArgs->SyncState = syncState;

	FiberTaskingLib::ThreadType renderThread;
	if (!FiberTaskingLib::CreateThread(1048576, RenderThreadStart, renderArgs, 0, &renderThread)) {
		printf("Failed to start the render system");
	}

	

	// Begin the logic loop
	rmt_SetCurrentThreadName("Main");
	FiberTaskingLib::TaskScheduler taskScheduler;

	ThreadArgs *logicArgs = new ThreadArgs;
	logicArgs->Window = window;
	logicArgs->SyncState = syncState;

	// This will block until we're ready to quit
	taskScheduler.Run(512, LogicMain, logicArgs, FiberTaskingLib::GetNumHardwareThreads() - 1);


	// Signal the render thread to terminate
	syncState->ShouldQuit.store(true);
	// Push a new scene to make sure the render thread doesn't deadlock
	syncState->SceneQueue.Push(0);

	// Wait for them to clean up
	FiberTaskingLib::JoinThread(renderThread);

	// Quit
	glfwTerminate();

	return 0;
}




