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
	program.SetShadersFileName(shadersPath + verts[shader_idx],
            shadersPath + frags[shader_idx]);

	program.Init();

}


void RenderEngine::Display()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	// glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (gameEngine->HasChangedScene())
	{
		// TODO reinitialize shadow maps
		gameEngine->ChangedSceneAcknowledged();
	}
	const auto& scene = gameEngine->GetCurrScene();
	camera = scene->GetCurrCamera();

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();


	for (const auto& model : scene->GetModels())
	{
		const auto& objTransform = std::dynamic_pointer_cast<Transform>( model->components[TRANSFORM]);
		const auto& objMaterial = std::dynamic_pointer_cast<Material>( model->components[MATERIAL]);
		const auto& objModel = std::dynamic_pointer_cast<Model>(model->components[MODEL]);

		std::string& modelPath = objModel->modelPath;

		if (posBuffMap.find(modelPath) == posBuffMap.end())
		{
			LoadModel(modelPath);
		}

		glm::mat4 modelMatrix(1.0f);
		// modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::translate(glm::mat4(1.0f), objTransform->position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f))	// TODO Refactor this
			* glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f))	// TODO Refactor this
			* glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f))	// TODO Refactor this
			* glm::scale(glm::mat4(1.0f), objTransform->scale);

    	glm::mat4 modelInverseTranspose = glm::transpose(glm::inverse(modelMatrix));
		program.Bind();
		program.SendAttributeData(posBuffMap[modelPath], "vPositionModel");
		program.SendAttributeData(norBuffMap[modelPath], "vNormalModel");
		program.SendUniformData(modelMatrix, "model");
		program.SendUniformData(viewMatrix, "view");
		program.SendUniformData(projectionMatrix, "projection");
		program.SendUniformData(modelInverseTranspose, "modelInverseTranspose");

    	if (0 <= shader_idx && shader_idx < NUM_SHADERS-1) {
		    program.SendUniformData(objMaterial->ambient, "ka");
		    program.SendUniformData(objMaterial->diffuse, "kd");
		    program.SendUniformData(objMaterial->specular, "ks");
		    program.SendUniformData(objMaterial->shininess, "s");

    		const auto& lights = scene->GetLights();

    		for (size_t i = 0; i < lights.size(); i++)
    		{
    			const auto& light = lights[i];
    			const auto lightTransform = std::dynamic_pointer_cast<Transform>(light->components[TRANSFORM]);
    			const auto lightComponent = std::dynamic_pointer_cast<Light>(light->components[LIGHT]);

    			std::string name = fmt::format("lights[{}]", i);

				program.SendUniformData(lightTransform->position, (name + ".position").c_str());
				program.SendUniformData(lightComponent->color, (name+".color").c_str());
    		}
    	}
		glDrawArrays(GL_TRIANGLES, 0, posBuffMap[modelPath].size() / 3);
		program.Unbind();
	}

}

void RenderEngine::CharacterCallback(GLFWwindow* window, unsigned int key)
{
	switch (key)
	{
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
