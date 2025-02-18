
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>

#include "gui/gui_engine.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <memory>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "core/game_engine.h"
#include "render/render_engine.h"


#include <yaml-cpp/yaml.h> // for tests, remove later
#include <serial/lights.h>
#include <serial/models.h>
#include "core/scene.h"
#include "serial/scenes.h"
#include "serial/project.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

GLFWwindow *window;

std::unique_ptr<GuiEngine> guiEngine;
std::unique_ptr<RenderEngine> renderEngine;

GameEngine gameEngine;

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	renderEngine->CharacterCallback(lWindow, key);
}
void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	renderEngine->FrameBufferSizeCallback(lWindow, width, height);
}


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char *argv[])
{	
	// GLFWwindow* window is shared between gui and render,
	// so let's declare it in main.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
	    return -1;
    }
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TBDEngine", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	guiEngine = std::make_unique<GuiEngine>();
	renderEngine = std::make_unique<RenderEngine>(window, &gameEngine);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Create a texture to attach to the framebuffer
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// Create a renderbuffer for depth and stencil attachment (optional)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int width,height;
	guiEngine->init(window, &gameEngine);
	while ( glfwWindowShouldClose(window) == 0 )
	{
		glfwGetWindowSize(window, &width, &height);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		guiEngine->run(textureColorbuffer, rbo);
		if(guiEngine->showView) {
			renderEngine->Display(framebuffer, width, height);
		}
		glfwSwapBuffers(window);
	}
	guiEngine->cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
	auto node = YAML::Node();
	node = SerializeProject(&gameEngine);
	std::ofstream yamlFile("../user/project.yaml");
	if(!yamlFile) {
		std::cout << "file does not exist" << std::endl;
		return 1;
	}
	yamlFile << node;
	yamlFile.close();



	return 0;
}