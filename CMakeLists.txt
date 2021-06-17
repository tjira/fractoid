cmake_minimum_required(VERSION 3.13)
project(Fractoid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS Forms)

include_directories(include)

find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED Widgets)

add_executable(fractoid
        src/main.cpp
        include/Fractal.h
        include/MainWindow.h
        include/SettingsWindow.h
        src/Fractal.cpp
        src/MainWindow.cpp
        src/SettingsWindow.cpp
        src/InteractiveLabel.cpp
        include/InteractiveLabel.h
)

target_link_libraries(fractoid ${OpenCV_LIBS} Qt5::Widgets)
