#pragma once

#include "shader.h"
#include "core/camera.h"
#include "core/game_engine.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class RenderEngine
{
	GLFWwindow* window;
	const std::string modelPath = "../resources/models/";
	const std::string shadersPath = "../resources/shaders/";

	std::string verts[2] = {
	    "vert_gradient.glsl",
		"vert_gradient.glsl",
	};

	std::string frags[2] = {
	    "frag_gradient.glsl",
	    "frag_gradient_box.glsl",
	};

	Shader currShader;
	GameEngine* gameEngine;

	// Vertex data: positions and texture coordinates for the rectangle
	GLfloat vertices[30] = {
		// Positions        // Texture Coordinates
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // Top-left
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // Top-right
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // Top-left
	};
	GLuint VBO, VAO;

public:
	RenderEngine(GLFWwindow* _window, GameEngine* _gameEngine)
	{
		window = _window;
		gameEngine = _gameEngine;

		Init();
	}

	void Init();

	void ShadersInit();
	void Display(unsigned int& framebuffer, int framebufferWidth, int framebufferHeight);
	void CharacterCallback(GLFWwindow* window, unsigned int key);
	void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height);

};
