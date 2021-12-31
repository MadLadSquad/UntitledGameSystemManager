cmake_minimum_required(VERSION 3.19)
project(ugm-gui)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../Common/Core/)
include_directories(../../Common/Core/Core/)
include_directories(glfw/include/GLFW)
include_directories(../../ThirdParty/imgui/misc)
include_directories(../../ThirdParty/imgui/backends/everything)
include_directories(../../ThirdParty/logger/src)
include_directories(../../ThirdParty)

add_subdirectory(glfw)
add_subdirectory(glew)

file(GLOB_RECURSE SRC "../../Common/*.cpp" "../../Common/*.hpp" "../../ThirdParty/imgui/backends/everything/*.cpp" "../../ThirdParty/imgui/backends/everything/*.h" "../../ThirdParty/imgui/misc/cpp/*.cpp" "../../ThirdParty/imgui/misc/cpp/*.h" "../../ThirdParty/logger/*.h" "../../ThirdParty/logger/*.cpp")

add_executable(ugm-gui main.cpp GUI.cpp GUI.hpp Commands.cpp Commands.hpp ${SRC})
set_target_properties(ugm-gui PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ugm-gui glfw GLEW OpenGL util pthread)