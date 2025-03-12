#pragma once

#include "render/tiny_obj_loader.h"

#include "render/Shader.h"

#include "core/components/component.h"
#include "core/components/light.h"
#include "core/components/transform.h"

struct Triangle
{
    alignas(16) glm::vec3 posA;
    alignas(16) glm::vec3 posB;
    alignas(16) glm::vec3 posC;
    alignas(16) glm::vec3 normalA;
    alignas(16) glm::vec3 normalB;
    alignas(16) glm::vec3 normalC;
    alignas(8) glm::vec2 uvA;
    alignas(8) glm::vec2 uvB;
    alignas(8) glm::vec2 uvC;
};

struct ModelInfo
{
    alignas(4) int nodeOffset;
    alignas(4) int triOffset;
    alignas(4) float pad1, pad2;
    alignas(16) glm::mat4x4 worldToLocalMatrix;
    alignas(16) glm::mat4x4 localToWorldMatrix;
    alignas(16) MaterialInfo material;
};

struct BoundingBox
{
    glm::vec3 Min;
    glm::vec3 Max;
    glm::vec3 Center = (Min + Max) * 0.5f;
    glm::vec3 Size = Max - Min;
    bool hasPoint;

    void GrowToInclude(glm::vec3 min, glm::vec3 max)
    {
        if(hasPoint)
        {
            Min = glm::min(Min, min);
            Max = glm::max(Max, max);
        }
        else
        {
            hasPoint = true;
            Min = min;
            Max = max;
        }
    }
};

struct BVHNode
{
    alignas(16) glm::vec3 boundsMin;
    alignas(16) glm::vec3 boundsMax;
    alignas(4) int startIndex;
    alignas(4) int triangleCount;
};

struct aabb
{
    glm::vec3 bmin = glm::vec3(1e30f), bmax = glm::vec3(-1e30f);
    void grow( glm::vec3 p ) { bmin = min( bmin, p ), bmax = max( bmax, p ); }
    float area()
    {
        glm::vec3 e = bmax - bmin; // box extent
        return e.x * e.y + e.y * e.z + e.z * e.x;
    }
};

class Basic_Shader : public Shader
{

    std::shared_ptr<Camera> camera;
    std::vector<Triangle> triangles;
    std::vector<unsigned int> triIndexs;
    std::vector<ModelInfo> models;
    std::unordered_map<std::string, int> loadedMeshOffset;
    std::unordered_map<std::string, bool> modelsLoaded;
    std::vector<BVHNode> bvhNodes;

    void BuildBVH(unsigned int triStartIndex, unsigned int triCount)
    {
        BVHNode root{glm::vec3(glm::uintBitsToFloat(0x7F800000)), glm::vec3(glm::uintBitsToFloat(0xFF800000)),-1,-1};
        for(size_t i = 0; i < triCount; i++)
        {
            root.boundsMin = glm::min(root.boundsMin, (triangles)[triStartIndex + i].posA);
            root.boundsMin = glm::min(root.boundsMin, triangles[triStartIndex + i].posB);
            root.boundsMin = glm::min(root.boundsMin, triangles[triStartIndex + i].posC);
            root.boundsMax = glm::max(root.boundsMax, triangles[triStartIndex + i].posA);
            root.boundsMax = glm::max(root.boundsMax, triangles[triStartIndex + i].posB);
            root.boundsMax = glm::max(root.boundsMax, triangles[triStartIndex + i].posC);
        }
        bvhNodes.push_back(root);
    }

    float NodeCost(glm::vec3 size, unsigned int triNum)
    {
        float halfArea = size.x * size.y + size.x * size.z + size.y * size.z;
        return halfArea * triNum;
    }

    struct BoundingBox
    {
        glm::vec3 Min;
        glm::vec3 Max;
        glm::vec3 Center = (Min + Max) * 0.5f;
        glm::vec3 Size = Max - Min;
        bool hasPoint;

    void GrowToInclude(Triangle tri)
        {
            if (hasPoint)
            {
                Min = min(Min, tri.posA);
                Min = min(Min, tri.posB);
                Min = min(Min, tri.posC);
                Max = max(Max, tri.posA);
                Max = max(Max, tri.posB);
                Max = max(Max, tri.posC);
            }
            else
            {
                hasPoint = true;
                Min = min(tri.posA,min(tri.posB, tri.posC));
                Max = max(tri.posA,max(tri.posB, tri.posC));
            }
        }
    };

    glm::vec3 GetTriCenter(Triangle tri)
    {
        glm::vec3 center = tri.posA + tri.posB + tri.posC;
        return center / 3.f;
    }

    float EvaluateSplit(int splitAxis, float splitPos, int start, int count)
    {
        int numOnLeft = 0, numOnRight = 0;
        BoundingBox boundsLeft, boundsRight;

        for(int i = start; i < start + count; ++i)
        {
            Triangle tri = triangles[i];
            if (GetTriCenter(tri)[splitAxis] < splitPos)
            {
                boundsLeft.GrowToInclude(tri);
                ++numOnLeft;
            }
            else
            {
                boundsRight.GrowToInclude(tri);
                ++numOnRight;
            }
        }

        float costA = NodeCost(boundsLeft.Size, numOnLeft);
        float costB = NodeCost(boundsRight.Size, numOnLeft);
        return costA + costB;
    }

    struct SplitCalc
    {
        int axis;
        float pos;
        float cost;
    };

    SplitCalc ChooseSplit(BVHNode node, int start, int count)
    {
        if (count <= 1)
        {
            return SplitCalc{0,0, glm::uintBitsToFloat(0x7F800000)};
        }

        float bestSplitPos = 0;
        int bestSplitAxis = 0;
        const int numSplitTests = 5;

        float bestCost = glm::uintBitsToFloat(0X7F800000);

        for(int axis = 0; axis < 3; ++axis)
        {
            for(int i = 0; i < numSplitTests; ++i)
            {
                float splitT = (i + 1.f)/(numSplitTests + 1.f);
                float splitPos = node.boundsMin[axis] + ((node.boundsMax[axis] - node.boundsMin[axis]) * splitT);
                float cost = EvaluateSplit(axis, splitPos, start, count);
                if (cost < bestCost)
                {
                    bestCost = cost;
                    bestSplitAxis = axis;
                    bestSplitPos = splitPos;
                }
            }
        }
        return SplitCalc{bestSplitAxis, bestSplitPos, bestCost};
    }

    void Split(int parentIndex, int triStartIndex, int triCount, int depth = 0)
    {
        const int MaxDepth = 64;
        BVHNode parent = bvhNodes[parentIndex];
        glm::vec3 size = parent.boundsMax - parent.boundsMin;
        float parentCost = NodeCost(size, triCount);

        SplitCalc result = ChooseSplit(parent, triStartIndex, triCount);

        if (depth < MaxDepth && result.cost < parentCost)
        {
            BoundingBox boundsLeft, boundsRight;
            int numOnLeft;

            for (int i = triStartIndex; i < triStartIndex + triCount; ++i)
            {
                Triangle tri = triangles[triIndexs[i]];
                if(GetTriCenter(tri)[result.axis] < result.pos)
                {
                    boundsLeft.GrowToInclude(tri);
                    int swap = triIndexs[triStartIndex + numOnLeft];
                    triIndexs[triStartIndex + numOnLeft++] = triIndexs[i];
                    triIndexs[i] = swap;
                }
                else
                {
                    boundsRight.GrowToInclude(tri);
                }
            }

            int numOnRight = triCount - numOnLeft;
            int triStartLeft = triStartIndex;
            int triStartRight = triStartIndex + numOnLeft;

            int childIndexLeft = bvhNodes.size();
            bvhNodes.push_back(BVHNode{boundsLeft.Min, boundsLeft.Max, triStartLeft, 0});
            int childIndexRight = bvhNodes.size();
            bvhNodes.push_back(BVHNode{boundsRight.Min, boundsRight.Max, triStartLeft, 0});

            parent.startIndex = childIndexLeft;
            bvhNodes[parentIndex] = parent;

            Split(childIndexLeft, triStartIndex, numOnLeft, depth + 1);
            Split(childIndexRight, triStartIndex + numOnLeft, numOnRight, depth + 1);
        }
        else
        {
            parent.startIndex = triStartIndex;
            parent.triangleCount = triCount;
            bvhNodes[parentIndex] = parent;
        }
    }

    // Function to generate a normal for a face (using the cross product of two edges)
    static glm::vec3 GenerateNormal(const std::vector<glm::vec3>& faceVertices)
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
        const std::string& meshName(name);
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

            // Temporary storage for normals if not present in the OBJ file
            std::vector<glm::vec3> generatedNormals;
            loadedMeshOffset[meshName] = triangles.size();

            // Loop over shapes
            for (auto & shape : shapes) {
                // Loop over faces (polygons)
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                    size_t fv = shape.mesh.num_face_vertices[f];
                    Triangle tri{};
                    std::vector<glm::vec3> position;
                    std::vector<glm::vec3> normal;
                    std::vector<glm::vec2> uv;
                    // Loop over vertices in the face.
                    std::vector<glm::vec3> faceVertices;  // To store face vertices for normal calculation
                    for (size_t v = 0; v < fv; v++) {
                        // Access to vertex
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                        float vx = attrib.vertices[3 * idx.vertex_index + 0];
                        float vy = attrib.vertices[3 * idx.vertex_index + 1];
                        float vz = attrib.vertices[3 * idx.vertex_index + 2];
                        position.push_back(glm::vec3(vx, vy, vz));

                        // Store the vertex for normal calculation (if normals are absent)
                        faceVertices.push_back(glm::vec3(vx, vy, vz));

                        if (!attrib.normals.empty()) {
                            // If normals are already provided, add them
                            // norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
                            // norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
                            // norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
                            glm::vec3 tempnormal(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2]);
                            normal.push_back(tempnormal);
                        }

                        if (!attrib.texcoords.empty()) {
                            // texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                            // texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                            uv.push_back(glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0], attrib.texcoords[2 * idx.texcoord_index + 1]));
                        }
                    }

                    index_offset += fv;
                    // Generate normals if not provided
                    if (attrib.normals.empty()) {
                        glm::vec3 tempnormal = GenerateNormal(faceVertices);
                        for (size_t v = 0; v < fv; v++) {
                            // Duplicate normal for each vertex in the face (flat shading)
                            // norBuff.push_back(normal.x);
                            // norBuff.push_back(normal.y);
                            // norBuff.push_back(normal.z);
                            normal.push_back(tempnormal);
                        }
                    }

                    tri.posA = position[0];
                    tri.posB = position[1];
                    tri.posC = position[2];

                    tri.normalA = normal[0];
                    tri.normalB = normal[1];
                    tri.normalC = normal[2];

                    if(!uv.empty())
                    {
                        tri.uvA = uv[0];
                        tri.uvB = uv[1];
                        tri.uvC = uv[2];
                    }
                    else
                    {
                        tri.uvA = glm::vec2(0);
                        tri.uvB = tri.uvA;
                        tri.uvC = tri.uvB;
                    }

                    triangles.push_back(tri);
                    triIndexs.push_back(triIndexs.size());
                }
            }
        }
    }

    template <typename T>
    void SendBufferData(std::vector<T>& buffer, const char* name, GLuint bindingIndex) {
        if (buffer.empty()) {
            std::cerr << "Empty buffer for attribute: " << name << std::endl;
            return;
        }

        GLuint bufferID = 0;

        // Check if the buffer exists for the given name
        if (bufferMap.find(name) == bufferMap.end()) {
            glGenBuffers(1, &bufferID);  // Generate a new buffer
            bufferMap[name] = bufferID;  // Store the buffer ID in the map
        } else {
            bufferID = bufferMap[name];  // Reuse the existing buffer ID
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);  // Bind SSBO
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * buffer.size(), buffer.data(), GL_STATIC_DRAW);  // Upload data
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, bufferID);  // Bind to specific binding point
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);  // Unbind
    }

public:


    void UpdateData() override {
        camera = scene->GetCurrCamera();
        if (!camera) // Check for a null camera
        {
            std::cerr << "No camera available, skipping render." << std::endl;
            glDrawArrays(GL_POINTS, 0, 0);
            return;
        }

        // Send Camera world space position
        SendUniformData(camera->GetPosition(), "_WorldSpaceCamPos");

        // Compute Camera's Local-to-World matrix (inverse of ViewMatrix)
        glm::mat4 CamLocalToWorldMatrix = glm::inverse(camera->GetViewMatrix());
        SendUniformData(CamLocalToWorldMatrix, "camLocalToWorldMatrix");

        // Compute the height and width of the view plane
        float fovy = glm::clamp(camera->GetFovy(), 1.0f, 179.0f); // Clamping FOV for safety
        float focusDist = camera->GetFocusDist();
        float aspect = camera->GetAspect();

        float planeHeight = focusDist * tan(glm::radians(fovy * 0.5f)) * 2.0f;
        float planeWidth = planeHeight * aspect;

        // Send viewParams as (width, height, focus distance)
        SendUniformData(glm::vec3(planeWidth, planeHeight, focusDist), "viewParams");

        for (const auto& model : scene->GetModels())
        {
            if(modelsLoaded.find(model->name) == modelsLoaded.end())
            {
                const auto& objTransform = std::dynamic_pointer_cast<Transform>( model->components[TRANSFORM]);
                const auto& objMaterial = std::dynamic_pointer_cast<Material>( model->components[MATERIAL]);
                const auto& objModel = std::dynamic_pointer_cast<Model>(model->components[MODEL]);

                std::string& modelPath = objModel->modelPath;

                // Check if the position buffer is already loaded
                if (loadedMeshOffset.find(modelPath) == loadedMeshOffset.end()) {
                    // Load model buffers if they are not already loaded
                    LoadModel(modelPath);
                    int offset = loadedMeshOffset[modelPath];
                    BuildBVH(offset, triangles.size() - offset);
                }

                glm::mat4 modelMatrix(1.0f);
                modelMatrix = glm::translate(glm::mat4(1.0f), objTransform->position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f))
                              * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f))
                              * glm::rotate(glm::mat4(1.0f), glm::radians(objTransform->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f))
                              * glm::scale(glm::mat4(1.0f), objTransform->scale);

                ModelInfo currentModel{};
                currentModel.triOffset = loadedMeshOffset[modelPath];
                currentModel.localToWorldMatrix = modelMatrix;
                currentModel.worldToLocalMatrix = glm::inverse(modelMatrix);
                currentModel.material = objMaterial->getMaterial();

                models.push_back(currentModel);

                modelsLoaded[model->name] = true;
            }
// Handle lights
//            const auto& lights = scene->GetLights();
//            for (size_t i = 0; i < lights.size(); i++) {
//                const auto& light = lights[i];
//                const auto lightTransform = std::dynamic_pointer_cast<Transform>(light->components[TRANSFORM]);
//                const auto lightComponent = std::dynamic_pointer_cast<Light>(light->components[LIGHT]);
//
//                std::string name = fmt::format("lights[{}]", i);
//                this->SendUniformData(lightTransform->position, (name + ".position").c_str());
//                this->SendUniformData(lightComponent->color, (name+".color").c_str());
//            }
        }

        SendUniformData((int)models.size(), "modelCount");
        SendBufferData(models, "models", 0);
        SendUniformData((int)triangles.size(), "triangleCount");
        SendBufferData(triangles, "triangles", 1);
        SendBufferData(triIndexs, "triIndex", 1);

//        std::cout << models[0].material.specularColor[0] << " " << models[0].material.specularColor[1] << " " << models[0].material.specularColor[2] << " " << models[0].material.specularColor[3] << std::endl;
    };
};