#include "render/render_engine.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "render/tiny_obj_loader.h"

#include "core/components/component.h"
#include "core/components/transform.h"

void RenderEngine::Init()
{
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

	// Lights
	lights[0].position = {0.0f, 0.0f, 3.0f};
	lights[0].color    = {0.5f, 0.5f, 0.5f};

	lights[1].position = {0.0f, 3.0f, 0.0f};
	lights[1].color    = {0.2f, 0.2f, 0.2f};

	ShadersInit();
}

void RenderEngine::LoadModel(const std::string &name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;

	posBuffMap[meshName] = std::vector<float>();
	norBuffMap[meshName] = std::vector<float>();

	std::vector<float>& posBuff = posBuffMap[meshName];
	std::vector<float>& norBuff = norBuffMap[meshName];

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

void RenderEngine::ShadersInit() {
	program.SetShadersFileName(resource_path + verts[shader_idx],
            resource_path + frags[shader_idx]);

	program.Init();

}


void RenderEngine::Display()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	// glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();


	for (const auto gameObj : gameEngine->GetGameObjects())
	{
		auto objTransform = std::dynamic_pointer_cast<Transform>( gameObj->components.at(0) );
		std::string& model_path = objTransform->model_path;

		if (posBuffMap.find(model_path) == posBuffMap.end())
		{
			LoadModel(model_path);
		}

		glm::mat4 modelMatrix(1.0f);
		// modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::translate(glm::mat4(1.0f), objTransform->position) * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), objTransform->scale);

    	glm::mat4 modelInverseTranspose = glm::transpose(glm::inverse(modelMatrix));
		program.Bind();
		program.SendAttributeData(posBuffMap[model_path], "vPositionModel");
		program.SendAttributeData(norBuffMap[model_path], "vNormalModel");
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
		glDrawArrays(GL_TRIANGLES, 0, posBuffMap[model_path].size() / 3);
		program.Unbind();
	}

}

void RenderEngine::CharacterCallback(GLFWwindow* window, unsigned int key)
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


void RenderEngine::FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}
