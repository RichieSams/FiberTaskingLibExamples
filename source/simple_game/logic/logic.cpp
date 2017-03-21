/* FiberTaskingLibExamples
 *
 * This is a set of examples to accompany the FiberTaskingLib
 * https://github.com/RichieSams/FiberTaskingLib
 *
 * FiberTaskingLibExamples is the legal property of Adrian Astley
 * Copyright Adrian Astley 2016
 */

#include "logic/logic.h"

#include "logic/game_sync_state.h"

#include <fiber_tasking_lib/task_scheduler.h>

#include <GLFW/glfw3.h>

#include <memory_lib/tagged_heap.h>

#include <remotery/remotery.h>


void Update(uint64 frameNumber, MemoryLib::TaggedHeap *heap);

FTL_TASK_ENTRY_POINT(LogicMain) {
	ThreadArgs *logicArgs = static_cast<ThreadArgs *>(arg);
	GLFWwindow *window = logicArgs->Window;
	GameSyncState *syncState = logicArgs->SyncState;
	delete logicArgs;

	// Main loop
	glfwSetTime(0.0);
	double accumulatedTime = 0.0;
	const double updatePeriod = 0.030;
	const int maxUpdatesPerLoop = 4;
	double lastTime = 0.0;

	uint64 sceneNumber = 1; // The first scene is 1, not 0, so the renderer can easily check when to start
	MemoryLib::TaggedHeap heap(4096);

	// Loop until there is a quit message from the window or the user.
	while (!glfwWindowShouldClose(window)) {
		// Check for events
		glfwPollEvents();


		// Networking here
		// ...

		// Logic
		rmt_ScopedCPUSample(LogicLoop, 0);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		accumulatedTime += deltaTime;
		
		// Avoid spiral of death
		// Drop world time from the update in order to keep up
		if (accumulatedTime >= updatePeriod * maxUpdatesPerLoop) {
			accumulatedTime = updatePeriod * maxUpdatesPerLoop;
		}

		while (accumulatedTime >= updatePeriod) {
			accumulatedTime -= updatePeriod;
			Update(sceneNumber++, &heap);
		}

		rmt_BeginCPUSample(WaitForRenderer, 0);
		syncState->SceneQueue.Push(sceneNumber - 1);
		rmt_EndCPUSample();
	}
}


void Update(uint64 frameNumber, MemoryLib::TaggedHeap *heap) {
	rmt_ScopedCPUSample(Update, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


//// Handle events
//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
//	glfwSetWindowShouldClose(window, 1);
//}

//// WASD keyboard movement
//const float cameraMoveSpeed = 0.01f;
//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)) { bodyTranslation += Vector3(headToWorldMatrix * Vector4(0, 0, -cameraMoveSpeed, 0)); }
//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)) { bodyTranslation += Vector3(headToWorldMatrix * Vector4(0, 0, +cameraMoveSpeed, 0)); }
//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)) { bodyTranslation += Vector3(headToWorldMatrix * Vector4(-cameraMoveSpeed, 0, 0, 0)); }
//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)) { bodyTranslation += Vector3(headToWorldMatrix * Vector4(+cameraMoveSpeed, 0, 0, 0)); }
//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C)) { bodyTranslation.y -= cameraMoveSpeed; }
//if ((GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE)) || (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Z))) { bodyTranslation.y += cameraMoveSpeed; }

//// Keep the camera above the ground
//if (bodyTranslation.y < 0.01f) { bodyTranslation.y = 0.01f; }

//static bool inDrag = false;
//const float cameraTurnSpeed = 0.005f;
//if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
//	static double startX, startY;
//	double currentX, currentY;

//	glfwGetCursorPos(window, &currentX, &currentY);
//	if (inDrag) {
//		bodyRotation.y -= float(currentX - startX) * cameraTurnSpeed;
//		bodyRotation.x -= float(currentY - startY) * cameraTurnSpeed;
//	}
//	inDrag = true; startX = currentX; startY = currentY;
//} else {
//	inDrag = false;
//}

// Close the GL context and release all resources