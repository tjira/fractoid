cmake_minimum_required(VERSION 3.13)
project(Fractoid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS Forms)

find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED Widgets)

add_executable(fractoid
        Sources/main.cpp
        Headers/Fractal.h
        Headers/MainWindow.h
        Headers/SettingsWindow.h
        Sources/Fractal.cpp
        Sources/MainWindow.cpp
        Sources/SettingsWindow.cpp
)

target_link_libraries(fractoid ${OpenCV_LIBS} Qt5::Widgets)
