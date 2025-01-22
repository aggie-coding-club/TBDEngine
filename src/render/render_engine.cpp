#include "render/render_engine.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "render/tiny_obj_loader.h"

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
    glGenBuffers(1, &VBO);       // Create a VBO

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
	currShader.SetShadersFileName(shadersPath + verts[0],
            shadersPath + frags[0]);

	currShader.Init();
}

void RenderEngine::Display()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Bind shader program
    currShader.Bind();

    // Bind the VAO and draw the rectangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw two triangles (6 vertices)
    glBindVertexArray(0);

    // Unbind the shader program
    currShader.Unbind();

}








void RenderEngine::CharacterCallback(GLFWwindow* window, unsigned int key)
{
}


void RenderEngine::FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}
