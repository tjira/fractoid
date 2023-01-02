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

void Gui::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
