FLAGS = -std=c++17 -fopenmp -O3 -Wall
LIBS = -ldl -lglfw

ifeq ($(OS),Windows_NT)
	FLAGS += -static-libstdc++ -static-libgcc
	LIBS += -lgdi32 -lwinmm
endif

all: fractoid

# Link =================================================================================================================

fractoid: fractoid.o buffer.o callback.o canvas.o gui.o image.o shader.o glad.o imgui.o imgui_demo.o imgui_dilog.o imgui_draw.o imgui_glfw.o imgui_opengl.o imgui_tables.o imgui_widgets.o
	g++ $(FLAGS) -o $@ $^ $(LIBS)

fractoid.o: fractoid.cpp
	g++ $(FLAGS) -c -o $@ $^

# Project ==============================================================================================================

buffer.o: src/buffer.cpp
	g++ $(FLAGS) -c -o $@ $^

callback.o: src/callback.cpp
	g++ $(FLAGS) -c -o $@ $^

canvas.o: src/canvas.cpp
	g++ $(FLAGS) -c -o $@ $^

gui.o: src/gui.cpp
	g++ $(FLAGS) -c -o $@ $^

image.o: src/image.cpp
	g++ $(FLAGS) -c -o $@ $^

shader.o: src/shader.cpp
	g++ $(FLAGS) -c -o $@ $^

# GLAD =================================================================================================================

glad.o: lib/glad/gl.c
	g++ -c -o $@ $^

# ImGui ================================================================================================================

imgui.o: lib/imgui/imgui.cpp
	g++ -c -o $@ $^

imgui_demo.o: lib/imgui/imgui_demo.cpp
	g++ -c -o $@ $^

imgui_dilog.o: lib/imgui/ImGuiFileDialog.cpp
	g++ -c -o $@ $^

imgui_draw.o: lib/imgui/imgui_draw.cpp
	g++ -c -o $@ $^

imgui_glfw.o: lib/imgui/imgui_impl_glfw.cpp
	g++ -c -o $@ $^

imgui_opengl.o: lib/imgui/imgui_impl_opengl3.cpp
	g++ -c -o $@ $^

imgui_tables.o: lib/imgui/imgui_tables.cpp
	g++ -c -o $@ $^

imgui_widgets.o: lib/imgui/imgui_widgets.cpp
	g++ -c -o $@ $^

# Miscellaneous ========================================================================================================

clean:
	rm -rf fractoid .cache .clangd .makefile .vscode *.exe *.json *.o *.png
