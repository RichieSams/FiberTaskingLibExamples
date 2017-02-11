/* FiberTaskingLibExamples
*
* This is a set of examples to accompany the FiberTaskingLib
* https://github.com/RichieSams/FiberTaskingLib
*
* FiberTaskingLibExamples is the legal property of Adrian Astley
* Copyright Adrian Astley 2016
*/

#pragma once


struct RenderContext;
struct GLFWwindow;

GLFWwindow *InitOpenGL(int width, int height, const char *title);
void InitRenderContext(GLFWwindow *window, RenderContext **context);
void ShutdownRenderContext(RenderContext *context);
void RenderFrame(GLFWwindow *window, RenderContext *context, double deltaTime);
