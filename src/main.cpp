#include "vk/vk_engine.h"
#include "UI.h"

int main(int, char**)
{
    UI *ui = new UI();
    if (ui->init()) {
        return 1;
    }
    // Main loop
    while (!glfwWindowShouldClose(ui->window))
    {
        ui->run();
    }

    ui->UI_Clean();

    return 0;
}