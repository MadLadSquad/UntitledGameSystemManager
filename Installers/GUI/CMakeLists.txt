cmake_minimum_required(VERSION 3.19)
project(ugm-gui-installer)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../Common/Core/)
include_directories(../../Common/Core/Core/)
include_directories(glfw/include/GLFW)
include_directories(../../ThirdParty/imgui/misc)
include_directories(../../ThirdParty/imgui/backends/everything)
add_subdirectory(glfw)

file(GLOB_RECURSE SRC "../../Common/*.cpp" "../../Common/*.hpp" "../../ThirdParty/imgui/backends/everything/*.cpp" "../../ThirdParty/imgui/backends/everything/*.h" "../../ThirdParty/imgui/misc/cpp/*.cpp" "../../ThirdParty/imgui/misc/cpp/*.h" "ThirdParty/UntitledLog/*.h" "ThirdParty/UntitledLog/*.cpp")

add_executable(ugm-gui-installer main.cpp GUI.cpp GUI.hpp ${SRC})
set_target_properties(ugm-gui-installer PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ugm-gui-installer glfw OpenGL util pthread)