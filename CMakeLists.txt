cmake_minimum_required(VERSION 3.26)
project(3DViewer_v2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS src/sources/ui)

include_directories(src/includes/)


find_package(Qt6 REQUIRED COMPONENTS Core  Widgets OpenGLWidgets)

include_directories(src/includes)

add_executable(3DViewer_v2
        src/sources/Model.cc
        src/includes/Model.h
        src/sources/main.cc
        src/sources/MainView.cc
        src/includes/MainView.h
        src/sources/ui/MainView.ui
        src/sources/OpenGLWidget.cc
        src/includes/OpenGLWidget.h
        src/includes/config.h
        src/sources/Controller.cc
        src/includes/Controller.h
)

target_link_libraries(3DViewer_v2 PRIVATE Qt6::Core Qt6::Widgets Qt6::OpenGLWidgets)
