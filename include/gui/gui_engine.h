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
#include "core/game_engine.h"

class GuiEngine
{
private:
     ImGuiIO* io;
     ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
     GLFWwindow* window;
     GameEngine* gameEngine;

     ImFont* Inter_24;

public:
     int width, height;
     GuiEngine(){}
     ~GuiEngine(){};
     bool init(GLFWwindow *window , GameEngine *_game_engine);
     void run(int width, int height);
     void cleanup();
};