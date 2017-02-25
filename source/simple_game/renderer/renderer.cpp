/* FiberTaskingLibExamples
 *
 * This is a set of examples to accompany the FiberTaskingLib
 * https://github.com/RichieSams/FiberTaskingLib
 *
 * FiberTaskingLibExamples is the legal property of Adrian Astley
 * Copyright Adrian Astley 2016
 */

#include "renderer/renderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>


namespace Cube {
	const float position[][3] = {-.5f, .5f, -.5f, -.5f, .5f, .5f, .5f, .5f, .5f, .5f, .5f, -.5f, -.5f, .5f, -.5f, -.5f, -.5f, -.5f, -.5f, -.5f, .5f, -.5f, .5f, .5f, .5f, .5f, .5f, .5f, -.5f, .5f, .5f, -.5f, -.5f, .5f, .5f, -.5f, .5f, .5f, -.5f, .5f, -.5f, -.5f, -.5f, -.5f, -.5f, -.5f, .5f, -.5f, -.5f, .5f, .5f, -.5f, -.5f, .5f, .5f, -.5f, .5f, .5f, .5f, .5f, -.5f, -.5f, .5f, -.5f, -.5f, -.5f, .5f, -.5f, -.5f, .5f, -.5f, .5f};
	const float normal[][3] = {0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f};
	const float tangent[][4] = {1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f, -1.f, 1.f, -1.f, 0.f, 0.f, 1.f, -1.f, 0.f, 0.f, 1.f, -1.f, 0.f, 0.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f};
	const float texCoord[][2] = {0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f};
	const int   index[] = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
};

struct RenderContext {
	int FrameWidth, FrameHeight;
	GLuint Framebuffer;

	unsigned int NumIndices;

	GLuint MainShader;
	struct {
		GLuint ColorTexture;
		GLuint TrilinearSampler;
		GLint ColorTextureUniform;
		GLuint UniformBindingPoint;
		GLuint UniformBlock;
		GLint *UniformOffset;
	} MainShaderProperties;
	GLuint MainVAO;

	GLuint SkyShader;
	struct {
		GLint LightUniform;
		GLint ResolutionUniform;
		GLint WorldMatrixUniform;
		GLint InvProjectionMatrixUniform;
	} SkyShaderProperties;
	GLuint SkyVAO;
};

GLFWwindow *InitOpenGL(int width, int height, const char *title) {
	if (!glfwInit()) {
		std::fprintf(stderr, "ERROR: Could not start GLFW\n");
		std::exit(1);
	}

	// Without these, shaders actually won't initialize properly
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

	GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		std::fprintf(stderr, "ERROR: could not open window with GLFW\n");
		glfwTerminate();
		std::exit(2);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL newContext" << std::endl;
		std::exit(1);
	}

	// Clear startup errors
	while (glGetError() != GL_NONE) {
	}

	glfwSwapInterval(0);

	std::fprintf(stderr, "GPU: %s (OpenGL version %s)\n", (char *)glGetString(GL_RENDERER), (char *)glGetString(GL_VERSION));

	// Check for errors
	{
		const GLenum error = glGetError();
		assert(error == GL_NONE);
	}

	return window;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint pixelShader) {
	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		fprintf(stderr, "Error while linking\n\nError: %s\n", &infoLog[0]);
		assert(false);

		// Clean up the program
		glDeleteProgram(program);

		return GL_NONE;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, pixelShader);

	return program;
}

GLuint compileShader(GLenum shaderType, const std::string &source) {
	GLuint shader = glCreateShader(shaderType);
	const char* srcArray[] = {source.c_str()};

	glShaderSource(shader, 1, srcArray, NULL);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);

		fprintf(stderr, "Error while compiling\n %s\n\nError: %s\n", source.c_str(), &errorLog[0]);
		assert(false);

		// Clean up the shader
		glDeleteShader(shader);
		shader = GL_NONE;
	}

	return shader;
}

std::string loadTextFile(const char *filename) {
	std::stringstream buffer;
	buffer << std::ifstream(filename).rdbuf();
	return buffer.str();
}

void InitRenderContext(GLFWwindow *window, RenderContext **context) {
	RenderContext *newContext = *context = new RenderContext();

	glfwGetFramebufferSize(window, &newContext->FrameWidth, &newContext->FrameHeight);
	glViewport(0, 0, newContext->FrameWidth, newContext->FrameHeight);

	GLuint colorRenderTarget;
	GLuint depthRenderTarget;

	glGenFramebuffers(1, &newContext->Framebuffer);
	glGenTextures(1, &colorRenderTarget);
	glGenTextures(1, &depthRenderTarget);

	glBindTexture(GL_TEXTURE_2D, colorRenderTarget);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, newContext->FrameWidth, newContext->FrameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, depthRenderTarget);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, newContext->FrameWidth, newContext->FrameHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_FRAMEBUFFER, newContext->Framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorRenderTarget, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthRenderTarget, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Load vertex array buffers
	GLuint positionBuffer = GL_NONE;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::position), Cube::position, GL_STATIC_DRAW);

	GLuint texCoordBuffer = GL_NONE;
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::texCoord), Cube::texCoord, GL_STATIC_DRAW);

	GLuint normalBuffer = GL_NONE;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::normal), Cube::normal, GL_STATIC_DRAW);

	GLuint tangentBuffer = GL_NONE;
	glGenBuffers(1, &tangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::tangent), Cube::tangent, GL_STATIC_DRAW);

	const int numVertices = sizeof(Cube::position) / sizeof(Cube::position[0]);

	GLuint indexBuffer = GL_NONE;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Cube::index), Cube::index, GL_STATIC_DRAW);
	newContext->NumIndices = sizeof(Cube::index) / sizeof(Cube::index[0]);


	// Create the main shader
	GLuint mainVertexShader = compileShader(GL_VERTEX_SHADER, loadTextFile("main_vs.glsl"));
	GLuint mainPixelShader = compileShader(GL_FRAGMENT_SHADER, loadTextFile("main_ps.glsl"));
	newContext->MainShader = createShaderProgram(mainVertexShader, mainPixelShader);

	// Binding points for attributes and uniforms discovered from the shader
	const GLint positionAttribute = glGetAttribLocation(newContext->MainShader, "position");
	const GLint normalAttribute = glGetAttribLocation(newContext->MainShader, "normal");
	const GLint texCoordAttribute = glGetAttribLocation(newContext->MainShader, "texCoord");
	const GLint tangentAttribute = glGetAttribLocation(newContext->MainShader, "tangent");
	newContext->MainShaderProperties.ColorTextureUniform = glGetUniformLocation(newContext->MainShader, "colorTexture");

	const GLuint uniformBlockIndex = glGetUniformBlockIndex(newContext->MainShader, "Uniform");

	newContext->MainShaderProperties.UniformBindingPoint = 1;
	glUniformBlockBinding(newContext->MainShader, uniformBlockIndex, newContext->MainShaderProperties.UniformBindingPoint);

	glGenBuffers(1, &newContext->MainShaderProperties.UniformBlock);

	{
		// Allocate space for the uniform block buffer
		GLint uniformBlockSize;
		glGetActiveUniformBlockiv(newContext->MainShader, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
		glBindBuffer(GL_UNIFORM_BUFFER, newContext->MainShaderProperties.UniformBlock);
		glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, nullptr, GL_DYNAMIC_DRAW);
	}

	const GLchar *uniformName[] = {
		"Uniform.world",
		"Uniform.worldInvTranspose",
		"Uniform.projViewWorld",
		"Uniform.light",
		"Uniform.cameraPosition"
	};

	const int numBlockUniforms = sizeof(uniformName) / sizeof(uniformName[0]);
	#ifdef _DEBUG
		{
			GLint debugNumUniforms = 0;
			glGetProgramiv(newContext->MainShader, GL_ACTIVE_UNIFORMS, &debugNumUniforms);
			for (GLint i = 0; i < debugNumUniforms; ++i) {
				GLchar name[1024];
				GLsizei size = 0;
				GLenum type = GL_NONE;
				glGetActiveUniform(newContext->MainShader, i, sizeof(name), nullptr, &size, &type, name);
				std::cout << "Uniform #" << i << ": " << name << "\n";
			}
			assert(debugNumUniforms >= numBlockUniforms);
		}
	#endif

	// Map uniform names to indices within the block
	GLuint uniformIndex[numBlockUniforms];
	glGetUniformIndices(newContext->MainShader, numBlockUniforms, uniformName, uniformIndex);
	assert(uniformIndex[0] < 10000);

	// Map indices to byte offsets
	newContext->MainShaderProperties.UniformOffset = new GLint[numBlockUniforms];
	glGetActiveUniformsiv(newContext->MainShader, numBlockUniforms, uniformIndex, GL_UNIFORM_OFFSET, newContext->MainShaderProperties.UniformOffset);
	assert(newContext->MainShaderProperties.UniformOffset[0] >= 0);

	// Create the main VAO and bind the buffers to it
	glGenVertexArrays(1, &newContext->MainVAO);
	glBindVertexArray(newContext->MainVAO);

	// in position
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	// in normal
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);

	// in tangent
	if (tangentAttribute != -1) {
		// Only bind if used
		glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
		glVertexAttribPointer(tangentAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(tangentAttribute);
	}

	// in texCoord 
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texCoordAttribute);

	// indexBuffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	// Unbind the VAO
	glBindVertexArray(0);


	// Load the sky shader
	GLuint skyVertexShader = compileShader(GL_VERTEX_SHADER, loadTextFile("sky_vs.glsl"));
	GLuint skyPixelShader = compileShader(GL_FRAGMENT_SHADER, loadTextFile("sky_ps.glsl"));
	newContext->SkyShader = createShaderProgram(skyVertexShader, skyPixelShader);

	newContext->SkyShaderProperties.LightUniform = glGetUniformLocation(newContext->SkyShader, "light");
	newContext->SkyShaderProperties.ResolutionUniform = glGetUniformLocation(newContext->SkyShader, "resolution");
	newContext->SkyShaderProperties.WorldMatrixUniform = glGetUniformLocation(newContext->SkyShader, "worldMatrix");
	newContext->SkyShaderProperties.InvProjectionMatrixUniform = glGetUniformLocation(newContext->SkyShader, "invProjectionMatrix");

	glGenVertexArrays(1, &newContext->SkyVAO);

	// Load a texture map
	{
		int textureWidth, textureHeight, channels;
		unsigned char *data = stbi_load("color.bmp", &textureWidth, &textureHeight, &channels, 0);

		if (data == nullptr) {
			std::fprintf(stderr, "ERROR: Could not load color.bmp\n");
			std::exit(1);
		}

		glGenTextures(1, &newContext->MainShaderProperties.ColorTexture);
		glBindTexture(GL_TEXTURE_2D, newContext->MainShaderProperties.ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, textureWidth, textureHeight, 0, (channels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	{
		glGenSamplers(1, &newContext->MainShaderProperties.TrilinearSampler);
		glSamplerParameteri(newContext->MainShaderProperties.TrilinearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(newContext->MainShaderProperties.TrilinearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(newContext->MainShaderProperties.TrilinearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(newContext->MainShaderProperties.TrilinearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void ShutdownRenderContext(RenderContext *context) {
	delete context->MainShaderProperties.UniformOffset;
	delete context;
}

void RenderFrame(GLFWwindow *window, RenderContext *context, double deltaTime) {
	GLenum error = glGetError();
	assert(error == GL_NONE);

	const float nearPlaneZ = 0.1f;
	const float farPlaneZ = 100.0f;
	const float verticalFieldOfView = 45.0f * M_PI / 180.0f;
	const glm::vec3 cameraPosition(3, 3, 5);

	glm::mat4x4 worldMatrix, viewMatrix, projectionMatrix;

	viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(verticalFieldOfView, (float)context->FrameWidth / context->FrameHeight, nearPlaneZ, farPlaneZ);

	glBindFramebuffer(GL_FRAMEBUFFER, context->Framebuffer);
	glViewport(0, 0, context->FrameWidth, context->FrameHeight);

	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3 light = glm::normalize(glm::vec3(1.0f, 0.5f, 0.2f));

	// Draw the background
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	glUseProgram(context->SkyShader);
	glUniform3fv(context->SkyShaderProperties.LightUniform, GL_FALSE, glm::value_ptr(light));
	glUniform2f(context->SkyShaderProperties.ResolutionUniform, float(context->FrameWidth), float(context->FrameHeight));
	glUniformMatrix4fv(context->SkyShaderProperties.WorldMatrixUniform, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniformMatrix4fv(context->SkyShaderProperties.InvProjectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm::inverse(projectionMatrix)));
	glBindVertexArray(context->SkyVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	// Draw a mesh
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glUseProgram(context->MainShader);

	// uniform colorTexture (samplers cannot be placed in blocks)
	const GLint colorTextureUnit = 0;
	glActiveTexture(GL_TEXTURE0 + colorTextureUnit);
	glBindTexture(GL_TEXTURE_2D, context->MainShaderProperties.ColorTexture);
	glBindSampler(colorTextureUnit, context->MainShaderProperties.TrilinearSampler);
	glUniform1i(context->MainShaderProperties.ColorTextureUniform, colorTextureUnit);

	// Other uniforms in the interface block
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, context->MainShaderProperties.UniformBindingPoint, context->MainShaderProperties.UniformBlock);

		GLubyte* ptr = (GLubyte*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(ptr + context->MainShaderProperties.UniformOffset[0], glm::value_ptr(worldMatrix), sizeof(glm::mat4x4::col_type::value_type) * glm::mat4x4::col_type::length() * glm::mat4x4::length());
		memcpy(ptr + context->MainShaderProperties.UniformOffset[1], glm::value_ptr(glm::transpose(glm::inverse(worldMatrix))), sizeof(glm::mat4x4::col_type::value_type) * glm::mat4x4::col_type::length() * glm::mat4x4::length());
		memcpy(ptr + context->MainShaderProperties.UniformOffset[2], glm::value_ptr(projectionMatrix * viewMatrix * worldMatrix), sizeof(glm::mat4x4::col_type::value_type) * glm::mat4x4::col_type::length() * glm::mat4x4::length());
		memcpy(ptr + context->MainShaderProperties.UniformOffset[3], glm::value_ptr(light), sizeof(glm::vec4::value_type) * glm::vec4::length());
		memcpy(ptr + context->MainShaderProperties.UniformOffset[4], glm::value_ptr(cameraPosition), sizeof(glm::vec3::value_type) * glm::vec3::length());
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	glBindVertexArray(context->MainVAO);
	glDrawElements(GL_TRIANGLES, context->NumIndices, GL_UNSIGNED_INT, nullptr);


	// Mirror to the window
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
	glBlitFramebuffer(0, 0, context->FrameWidth, context->FrameHeight, 0, 0, context->FrameWidth, context->FrameHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);

	// Display what has been drawn on the main window
	glfwSwapBuffers(window);
}
