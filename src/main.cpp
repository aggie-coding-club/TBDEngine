#include "gui/gui_engine.h"

int main() {

  GUIEngine engine = GUIEngine();

  engine.init();

  while(!glfwWindowShouldClose(engine.window)) {
    engine.run();
  }

  engine.GUI_Clean();
}   