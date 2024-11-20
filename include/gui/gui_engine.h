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
#include "core/camera.h"

// Window classes
#include "Details.h"
#include "Viewport.h"
#include "FileHierarchy.h"
#include "MenuBar.h"

class GuiEngine
{
private:
     ImGuiIO* io;
     ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
     GLFWwindow* window;
     GameEngine* gameEngine;
     ImFont* inter_24;
     bool showDetail = true;
     bool showHierarchy = true;
     bool showCameraWindow = false;
     Camera *camera;
     bool showAddObject = false;
    // For Add object window
    int objectType = 0;

    char objectLocation[128];
    char objectName[128];

    // Windows
    Viewport* viewport;
    FileHierarchy* fileHierarchy;
    Details* details;
    MenuBar* menuBar;

public:
     bool showView = true;
     int width, height;
     GuiEngine(Camera *_camera) : camera(_camera){};
     ~GuiEngine(){};
     bool init(GLFWwindow *window , GameEngine *_game_engine);
     void run(int width, int height);
     void cleanup();
};
