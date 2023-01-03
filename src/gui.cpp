#include "../include/gui.h"

Gui::Gui(GLFWwindow* window) : window(window) {
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init("#version 420");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::GetIO().IniFilename = nullptr;
}

Gui::~Gui() {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

#include <iostream>

void Gui::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (fps) {
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::Begin("info", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoFocusOnAppearing
        );
        ImGui::Text("%.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (options) {
        GLFWPointer* pointer = (GLFWPointer*)glfwGetWindowUserPointer(window);

        ImGui::SetNextWindowSize({ 512, 288 }, ImGuiCond_FirstUseEver);
        ImGui::Begin("Options", &options);

        ImGui::SliderFloat("Bailout", &pointer->settings.escape.bailout, 0, 16);
        ImGui::SliderInt("Iters", &pointer->settings.escape.iterations, 0, 256);
        ImGui::SliderFloat3("Amplitude", &pointer->settings.periodic.r1, 0, 32);
        ImGui::SliderFloat3("Phase", &pointer->settings.periodic.r2, 0, 6.283);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
