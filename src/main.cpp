#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "render/Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "render/tiny_obj_loader.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "gui/gui_engine.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3
#define NUM_SHADERS 3

GLFWwindow *window;
std::string resource_path = "../resources";

std::string verts[NUM_SHADERS] = {
    "/vert.glsl",
    "/phong_vert.glsl",
    "/silhouette_vert.glsl"
};

std::string frags[NUM_SHADERS] = {
    "/frag.glsl",
    "/phong_frag.glsl",
    "/silhouette_frag.glsl"
};

Program program;
std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f);

struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} materials[NUM_MATERIALS];

struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
} lights[NUM_LIGHTS];

int mat_idx = 0;
int shader_idx = 0;

void ShadersInit();

void Display()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 modelInverseTranspose = glm::transpose(glm::inverse(modelMatrix));

	program.Bind();
	program.SendUniformData(modelMatrix, "model");
	program.SendUniformData(viewMatrix, "view");
	program.SendUniformData(projectionMatrix, "projection");
	program.SendUniformData(modelInverseTranspose, "modelInverseTranspose");

    if (0 <= shader_idx && shader_idx < NUM_SHADERS-1) {
	    program.SendUniformData(materials[mat_idx].ka, "ka");
	    program.SendUniformData(materials[mat_idx].kd, "kd");
	    program.SendUniformData(materials[mat_idx].ks, "ks");
	    program.SendUniformData(materials[mat_idx].s, "s");

	    program.SendUniformData(lights[0].position, "lights[0].position");
	    program.SendUniformData(lights[0].color, "lights[0].color");

	    program.SendUniformData(lights[1].position, "lights[1].position");
	    program.SendUniformData(lights[1].color, "lights[1].color");

    }

	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	program.Unbind();
}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key)
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
    case 'm':
        mat_idx = (++mat_idx) % NUM_MATERIALS;
        break;
    case 'M':
        mat_idx = (--mat_idx % NUM_MATERIALS + NUM_MATERIALS) % NUM_MATERIALS;
        break;
    case '3':
        shader_idx = 2;
        ShadersInit();
        break;
    case '2':
        shader_idx = 1;
        ShadersInit();
        break;
    case '1':
        shader_idx = 0;
        ShadersInit();
        break;
	default:
		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(const std::string &name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void SetMaterials() {
    materials[0].ka = {0.2f, 0.2f, 0.2f};
    materials[0].kd = {0.8f, 0.7f, 0.7f};
    materials[0].ks = {1.0f, 1.0f, 1.0f};
    materials[0].s  = 10.0f;

    materials[1].ka = {0.0f, 0.2f, 0.2f};
    materials[1].kd = {0.5f, 0.7f, 0.2f};
    materials[1].ks = {0.1f, 1.0f, 0.1f};
    materials[1].s  = 100.0;

    materials[2].ka = {0.2f, 0.2f, 0.2f};
    materials[2].kd = {0.1f, 0.3f, 0.9f};
    materials[2].ks = {0.1f, 0.1f, 0.1f};
    materials[2].s  = 1.0;
}

void SetLight() {
    lights[0].position = {0.0f, 0.0f, 3.0f};
    lights[0].color    = {0.5f, 0.5f, 0.5f};

    lights[1].position = {0.0f, 3.0f, 0.0f};
    lights[1].color    = {0.2f, 0.2f, 0.2f};
}
void ShadersInit() {
	program.SetShadersFileName(resource_path + verts[shader_idx],
            resource_path + frags[shader_idx]);

	program.Init();

	program.SendAttributeData(posBuff, "vPositionModel");
	program.SendAttributeData(norBuff, "vNormalModel");
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TBDEngine", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	SetMaterials();
	SetLight();

	LoadModel(resource_path + "/bunny.obj");

	ShadersInit();
	
}


int main(int argc, char *argv[])
{	
	Init();
	GuiEngine engine = GuiEngine();
	engine.init(window);
	while ( glfwWindowShouldClose(window) == 0 )
	{
		int width,height;
		glfwGetWindowSize(window, &width, &height);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		engine.run(width,height);
		Display();
		glfwSwapBuffers(window);
	}
	engine.cleanup();
	glfwTerminate();
	return 0;
}