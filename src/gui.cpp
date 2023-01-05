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
 
        ImGui::Begin("Options", &options, ImGuiWindowFlags_AlwaysAutoResize);

        static int fractal = 3, alg = 0, col = 1; int sum = fractal + alg + col;
        ImGui::Separator();
        if (ImGui::RadioButton("Buffalo", &fractal, 0)) { pointer->state.at(0) = "buffalo"; } ImGui::SameLine();
        if (ImGui::RadioButton("Burning Ship", &fractal, 1)) { pointer->state.at(0) = "burningship"; } ImGui::SameLine();
        if (ImGui::RadioButton("Julia", &fractal, 2)) { pointer->state.at(0) = "julia"; } 
        if (ImGui::RadioButton("Mandelbrot", &fractal, 3)) { pointer->state.at(0) = "mandelbrot"; } ImGui::SameLine();
        if (ImGui::RadioButton("Manowar", &fractal, 4)) { pointer->state.at(0) = "manowar"; } ImGui::SameLine();
        if (ImGui::RadioButton("Phoenix", &fractal, 5)) { pointer->state.at(0) = "phoenix"; }
        if (ImGui::CollapsingHeader("Julia", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(0);
            std::vector<float> c = { (float)pointer->settings.julia.c.real(), (float)pointer->settings.julia.c.imag() };
            ImGui::SliderFloat2("Parameter C", c.data(), -1, 1);
            pointer->settings.julia.c = { c.at(0), c.at(1) };
            ImGui::PopID();
        }
        if (ImGui::CollapsingHeader("Phoenix", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(0);
            std::vector<float> c = { (float)pointer->settings.phoenix.c.real(), (float)pointer->settings.phoenix.c.imag() };
            ImGui::SliderFloat2("Parameter C", c.data(), -1, 1);
            pointer->settings.phoenix.c = { c.at(0), c.at(1) };
            ImGui::PopID();
        }
        ImGui::Separator();
        if (ImGui::RadioButton("Escape", &alg, 0)) { pointer->state.at(1) = "escape"; } ImGui::SameLine();
        if (ImGui::RadioButton("Orbitrap", &alg, 1)) { pointer->state.at(1) = "orbitrap"; }
        if (ImGui::CollapsingHeader("Escape", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(0);
            ImGui::SliderFloat("Bailout", &pointer->settings.escape.bailout, 0, 16); ImGui::SameLine();
            ImGui::Checkbox("Smooth", &pointer->settings.escape.smooth);
            ImGui::SliderInt("Iters", &pointer->settings.escape.iterations, 0, 256);
            ImGui::PopID();
        }
        if (ImGui::CollapsingHeader("Orbitrap", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(1);
            ImGui::SliderFloat("Bailout", &pointer->settings.orbitrap.bailout, 0, 128); ImGui::SameLine();
            ImGui::Checkbox("Fill", &pointer->settings.orbitrap.fill);
            ImGui::SliderInt("Iters", &pointer->settings.orbitrap.iterations, 0, 256);
            ImGui::SliderInt("Trap", &pointer->settings.orbitrap.trap, 1, 5);
            ImGui::PopID();
        }
        ImGui::Separator();
        if (ImGui::RadioButton("Linear", &col, 0)) { pointer->state.at(2) = "linear"; } ImGui::SameLine();
        if (ImGui::RadioButton("Periodic", &col, 1)) { pointer->state.at(2) = "periodic"; } ImGui::SameLine();
        if (ImGui::RadioButton("Solid", &col, 2)) { pointer->state.at(2) = "solid"; };
        if (ImGui::CollapsingHeader("Linear", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(2);
            std::vector<float> from(pointer->settings.linear.from.begin(), pointer->settings.linear.from.end());
            std::vector<float> to(pointer->settings.linear.to.begin(), pointer->settings.linear.to.end());
            from.at(0) /= 255, from.at(1) /= 255, from.at(2) /= 255;
            to.at(0) /= 255, to.at(1) /= 255, to.at(2) /= 255;
            ImGui::ColorEdit3("From", from.data());
            ImGui::ColorEdit3("To", to.data());
            pointer->settings.linear.from = { int(255 * from.at(0)), int(255 * from.at(1)), int(255 * from.at(2)) };
            pointer->settings.linear.to = { int(255 * to.at(0)), int(255 * to.at(1)), int(255 * to.at(2)) };
            ImGui::PopID();
        }
        if (ImGui::CollapsingHeader("Periodic", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(3);
            std::vector<float> amplitude = { (float)pointer->settings.periodic.r1, (float)pointer->settings.periodic.g1, (float)pointer->settings.periodic.b1 };
            std::vector<float> phase = { (float)pointer->settings.periodic.r2, (float)pointer->settings.periodic.g2, (float)pointer->settings.periodic.b2 };
            ImGui::SliderFloat3("Amplitude", amplitude.data(), 0, 32);
            ImGui::SliderFloat3("Phase", phase.data(), 0, 6.283);
            pointer->settings.periodic.r1 = amplitude.at(0), pointer->settings.periodic.g1 = amplitude.at(1), pointer->settings.periodic.b1 = amplitude.at(2);
            pointer->settings.periodic.r2 = phase.at(0), pointer->settings.periodic.g2 = phase.at(1), pointer->settings.periodic.b2 = phase.at(2);
            ImGui::PopID();
        }
        if (ImGui::CollapsingHeader("Solid", ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
            ImGui::PushID(4);
            std::vector<float> rgb(pointer->settings.solid.rgb.begin(), pointer->settings.solid.rgb.end());
            rgb.at(0) /= 255, rgb.at(1) /= 255, rgb.at(2) /= 255;
            ImGui::ColorEdit3("Color", rgb.data());
            pointer->settings.solid.rgb = { int(255 * rgb.at(0)), int(255 * rgb.at(1)), int(255 * rgb.at(2)) };
            ImGui::PopID();
        }
        ImGui::Separator();
        if (fractal + alg + col != sum) {
            pointer->shader.~Shader(); new(&pointer->shader) Shader(vertex, generateFractalShader(pointer->state.at(0), pointer->state.at(1), pointer->state.at(2)));
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
