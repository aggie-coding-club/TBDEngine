#pragma once

#include "render/tiny_obj_loader.h"

#include "render/Shader.h"

#include "core/components/component.h"
#include "core/components/light.h"
#include "core/components/transform.h"

class Basic_Shader : public Shader{

    std::unordered_map<std::string, std::vector<float>> posBuffMap;
    std::unordered_map<std::string, std::vector<float>> texBuffMap;
    std::unordered_map<std::string, std::vector<float>> norBuffMap;

    std::shared_ptr<Camera> camera;

    // Function to generate a normal for a face (using the cross product of two edges)
    glm::vec3 GenerateNormal(const std::vector<glm::vec3>& faceVertices)
    {
        // Ensure there are exactly three vertices
        if (faceVertices.size() != 3) {
            std::cerr << "Error: Cannot generate normal for a non-triangle face." << std::endl;
            return glm::vec3(0.0f); // Return a zero normal
        }

        glm::vec3 v0 = faceVertices[0];
        glm::vec3 v1 = faceVertices[1];
        glm::vec3 v2 = faceVertices[2];

        // Calculate two edges
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        // Calculate the normal using the cross product
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        return normal;
    }

    void LoadModel(const std::string &name)
    {
        // Taken from Shinjiro Sueda with slight modification
        std::string meshName(name);
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string errStr;

        posBuffMap[meshName] = std::vector<float>();
        norBuffMap[meshName] = std::vector<float>();
        texBuffMap[meshName] = std::vector<float>(); // Assuming texBuffMap is declared somewhere

        std::vector<float>& posBuff = posBuffMap[meshName];
        std::vector<float>& norBuff = norBuffMap[meshName];
        std::vector<float>& texBuff = texBuffMap[meshName];

        bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
        if (!rc) {
            std::cerr << errStr << std::endl;
        }
        else {
            // Some OBJ files have different indices for vertex positions, normals,
            // and texture coordinates. For example, a cube corner vertex may have
            // three different normals. Here, we are going to duplicate all such
            // vertices.

            // Temporary storage for normals if not present in the OBJ file
            std::vector<glm::vec3> generatedNormals;

            // Loop over shapes
            for (auto & shape : shapes) {
                // Loop over faces (polygons)
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                    size_t fv = shape.mesh.num_face_vertices[f];
                    // Loop over vertices in the face.
                    std::vector<glm::vec3> faceVertices;  // To store face vertices for normal calculation
                    for (size_t v = 0; v < fv; v++) {
                        // Access to vertex
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                        float vx = attrib.vertices[3 * idx.vertex_index + 0];
                        float vy = attrib.vertices[3 * idx.vertex_index + 1];
                        float vz = attrib.vertices[3 * idx.vertex_index + 2];
                        posBuff.push_back(vx);
                        posBuff.push_back(vy);
                        posBuff.push_back(vz);

                        // Store the vertex for normal calculation (if normals are absent)
                        faceVertices.push_back(glm::vec3(vx, vy, vz));

                        if (!attrib.normals.empty()) {
                            // If normals are already provided, add them
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

                    // Generate normals if not provided
                    if (attrib.normals.empty()) {
                        glm::vec3 normal = GenerateNormal(faceVertices);
                        for (size_t v = 0; v < fv; v++) {
                            // Duplicate normal for each vertex in the face (flat shading)
                            norBuff.push_back(normal.x);
                            norBuff.push_back(normal.y);
                            norBuff.push_back(normal.z);
                        }
                    }
                }
            }
        }
    }

public:


    void UpdateData() {
        camera = scene->GetCurrCamera();
        if (!camera) // Better check for null camera
        {
            std::cerr << "No camera available, skipping render." << std::endl;
            glDrawArrays(GL_POINTS, 0, 0);
            return;
        }

        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
        glm::mat4 viewMatrix = camera->GetViewMatrix();

        for (const auto& model : scene->GetModels())
        {
            const auto& objTransform = std::dynamic_pointer_cast<Transform>( model->components[TRANSFORM]);
            const auto& objMaterial = std::dynamic_pointer_cast<Material>( model->components[MATERIAL]);
            const auto& objModel = std::dynamic_pointer_cast<Model>(model->components[MODEL]);

            std::string& modelPath = objModel->modelPath;

            // Check if the position buffer is already loaded
            if (posBuffMap.find(modelPath) == posBuffMap.end()) {
                // Load model buffers if they are not already loaded
                LoadModel(modelPath);
            }

            glm::mat4 modelMatrix(1.0f);
            modelMatrix = glm::translate(glm::mat4(1.0f), objTransform->position)
                * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f))
                * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f))
                * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f))
                * glm::scale(glm::mat4(1.0f), objTransform->scale);

            glm::mat4 modelInverseTranspose = glm::transpose(glm::inverse(modelMatrix));
            this->Bind();

            // Use existing buffers stored in posBuffMap
            this->SendAttributeData(posBuffMap[modelPath], "vPositionModel");
            this->SendAttributeData(norBuffMap[modelPath], "vNormalModel");
            this->SendUniformData(modelMatrix, "model");
            this->SendUniformData(viewMatrix, "view");
            this->SendUniformData(projectionMatrix, "projection");
            this->SendUniformData(modelInverseTranspose, "modelInverseTranspose");

            // Handle materials
            if (objMaterial) {
                this->SendUniformData(objMaterial->ambient, "ka");
                this->SendUniformData(objMaterial->diffuse, "kd");
                this->SendUniformData(objMaterial->specular, "ks");
                this->SendUniformData(objMaterial->shininess, "s");
            }

            // Handle lights
            const auto& lights = scene->GetLights();
            for (size_t i = 0; i < lights.size(); i++) {
                const auto& light = lights[i];
                const auto lightTransform = std::dynamic_pointer_cast<Transform>(light->components[TRANSFORM]);
                const auto lightComponent = std::dynamic_pointer_cast<Light>(light->components[LIGHT]);

                std::string name = fmt::format("lights[{}]", i);
                this->SendUniformData(lightTransform->position, (name + ".position").c_str());
                this->SendUniformData(lightComponent->color, (name+".color").c_str());
            }
        }

        this->SendUniformData(1.f,"tempcolor");
    };
};