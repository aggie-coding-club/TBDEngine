#include "render/render_engine.h"

#include "core/components/component.h"
#include "core/components/light.h"
#include "core/components/transform.h"

#include <fmt/core.h>

void RenderEngine::Init()
{
#ifndef _USE_SCENE_
	// Lights
	lights[0].position = {0.0f, 0.0f, 3.0f};
	lights[0].color    = {0.5f, 0.5f, 0.5f};

	lights[1].position = {0.0f, 3.0f, 0.0f};
	lights[1].color    = {0.2f, 0.2f, 0.2f};
#endif
    glGenVertexArrays(1, &VAO); // Create a VAO
    glGenBuffers(1, &VBO);      // Create a VBO

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and send vertex data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

	ShadersInit();
}

void RenderEngine::ShadersInit() {
	int currentShaderIDs = gameEngine->GetCurrScene()->current_shader;
	currShader.SetShadersFileName(shadersPath + verts[currentShaderIDs],shadersPath + frags[currentShaderIDs]);
	currShader.Init(gameEngine->GetCurrScene());
}

void RenderEngine::Display(unsigned int& framebuffer, int framebufferWidth, int framebufferHeight) {
	// Bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Ensure the viewport matches the framebuffer size
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Clear the framebuffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Verify framebuffer completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	// Bind the shader program
	currShader.Bind();

	currShader.SendUniformData(glm::vec2(framebufferWidth,framebufferHeight), "_ScreenParams");
	// Update uniforms, if necessary (e.g., transformation matrices)
	currShader.UpdateData();

	// Bind the VAO and draw
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Unbind shader
	currShader.Unbind();
}



void RenderEngine::CharacterCallback(GLFWwindow* window, unsigned int key)
{
	// Nothing rn cuz yea
}


void RenderEngine::FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}
