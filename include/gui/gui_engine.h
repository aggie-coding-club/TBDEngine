#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

class GuiEngine
{
private:
     ImGuiIO* io;
     ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

     ImFont* Inter_24;
public:
     GLFWwindow* window;
     int width, height;
     GuiEngine(){}
     ~GuiEngine(){};
     bool init(GLFWwindow *window);
     void run();
     void cleanup();
};