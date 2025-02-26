#pragma once
#include <iostream>
#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    float fovy;
    float aspect;
    float focusDistance;
    glm::vec3 Position;
    glm::vec4 Rotation;

public:

    Camera(const int width, const int height): fovy(45.0f),
        aspect(static_cast<float>(width)/ static_cast<float>(height)),
        focusDistance(1.0f),
        Position(glm::vec3(0.0f, 0.0f, 4.0f)),
        Rotation(glm::vec4(0.0f)) {}

    Camera(): Camera(1920, 1080){}

    inline glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, GetCenter(), GetUpVec());
    }

    inline glm::mat4 GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fovy), aspect, 0.1f, 100.0f);
    }

    inline float GetFovy() const
    {
        return fovy;
    }

    inline float GetAspect() const
    {
        return aspect;
    }

    inline float GetFocusDist() const
    {
        return focusDistance;
    }

    inline glm::vec3 GetPosition() const
    {
        return Position;
    }

    inline glm::vec3 GetRotation() const
    {
        glm::vec3 angles;

        float sinr_cosp = 2 * (Rotation.w * Rotation.x + Rotation.y * Rotation.z);
        float cosr_cosp = 1 - 2 * (Rotation.x * Rotation.x + Rotation.y * Rotation.y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        float sinp = std::sqrt(1 + 2 * (Rotation.w * Rotation.y - Rotation.x * Rotation.z));
        float cosp = std::sqrt(1 - 2 * (Rotation.w * Rotation.y - Rotation.x * Rotation.z));
        angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

        // yaw (z-axis rotation)
        float siny_cosp = 2 * (Rotation.w * Rotation.z + Rotation.x * Rotation.y);
        float cosy_cosp = 1 - 2 * (Rotation.y * Rotation.y + Rotation.z * Rotation.z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);

        return angles;
    }

    inline glm::vec3 GetCenter() const
    {
        glm::vec3 t = glm::cross(2.f *glm::vec3(Rotation.x,Rotation.y,Rotation.z), glm::vec3(0,0,focusDistance));
        return Position + glm::vec3(0,0,focusDistance) + Rotation.w * t + cross(glm::vec3(Rotation.x,Rotation.y,Rotation.z), t);
    }

    inline glm::vec3 GetUpVec() const
    {
        glm::vec3 t = glm::cross(2.f *glm::vec3(Rotation.x,Rotation.y,Rotation.z), glm::vec3(0,1,0));
        return glm::vec3(0,1,0) + Rotation.w * t + cross(glm::vec3(Rotation.x,Rotation.y,Rotation.z), t);
    }

    inline void SetAspect(const int width, const int height)
    {
        aspect = float(width) / float(height);
    }

    inline void SetFovy(const float _fovy)
    {
        fovy = glm::radians(_fovy);
    }

    inline void SetFocusDist(const float _focusDistance)
    {
        focusDistance = _focusDistance;
    }

    inline void SetPosition(const glm::vec3 _pos)
    {
        Position = _pos;
    }

    inline void SetRotation(const glm::vec3 _rot)
    {
        double cr = cos(_rot.x * 0.5);
        double sr = sin(_rot.x * 0.5);
        double cp = cos(_rot.y * 0.5);
        double sp = sin(_rot.y * 0.5);
        double cy = cos(_rot.z * 0.5);
        double sy = sin(_rot.z * 0.5);

        Rotation.w = cr * cp * cy + sr * sp * sy;
        Rotation.x = sr * cp * cy - cr * sp * sy;
        Rotation.y = cr * sp * cy + sr * cp * sy;
        Rotation.z = cr * cp * sy - sr * sp * cy;
    }
};