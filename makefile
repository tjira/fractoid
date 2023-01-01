FLAGS = -std=c++17 -O3 -Wall -fopenmp

ifeq ($(OS),Windows_NT)
	FLAGS += -static-libstdc++ -static-libgcc
endif

all: fractoid

# Link =================================================================================================================

fractoid: fractoid.o image.o timer.o uniform.o
	g++ $(FLAGS) -o $@ $^

fractoid.o: fractoid.cpp
	g++ $(FLAGS) -c -o $@ $^

# Project ==============================================================================================================

image.o: src/image.cpp
	g++ $(FLAGS) -c -o $@ $^

timer.o: src/timer.cpp
	g++ $(FLAGS) -c -o $@ $^

uniform.o: src/uniform.cpp
	g++ $(FLAGS) -c -o $@ $^

# Miscellaneous ========================================================================================================

clean:
	rm -rf compile_commands.json fractal.json fractoid Session.vim .cache .clangd .makefile .vscode *.exe *.o *.png
