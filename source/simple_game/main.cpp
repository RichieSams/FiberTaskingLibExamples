/* FiberTaskingLibExamples
 *
 * This is a set of examples to accompany the FiberTaskingLib
 * https://github.com/RichieSams/FiberTaskingLib
 *
 * FiberTaskingLibExamples is the legal property of Adrian Astley
 * Copyright Adrian Astley 2016
 */


#include "renderer/renderer.h"

#include <GLFW/glfw3.h>


void Update();

int main(const int argc, const char **argv) {
	GLFWwindow *window = InitOpenGL(1280, 720, "FiberTaskingLib Examples - Simple Game");
	RenderContext *context;
	InitRenderContext(window, &context);
	
	// Main loop
	glfwSetTime(0.0);
	double accumulatedTime = 0.0;
	double updatePeriod = 0.030;

	// Loop until there is a quit message from the window or the user.
	while (!glfwWindowShouldClose(window)) {
		// Check for events
		glfwPollEvents();

		// Otherwise do the frame processing
		double deltaTime = glfwGetTime();
		// Avoid spiral of death
		if (deltaTime > 250.0) {
			deltaTime = 250.0;
		}
		accumulatedTime += deltaTime;

		while (accumulatedTime >= updatePeriod) {
			accumulatedTime -= updatePeriod;
			Update();
		}

		RenderFrame(window, context, deltaTime);
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
	glfwTerminate();

	return 0;
}



void Update() {
	
}
