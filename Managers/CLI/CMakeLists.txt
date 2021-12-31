cmake_minimum_required(VERSION 3.19)
project(ugm-cli)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../Common/Core/)
include_directories(../../Common/Core/Core/)
include_directories(../../ThirdParty/logger/src)
include_directories(../../ThirdParty/yaml-cpp/include)
include_directories(../../ThirdParty/yaml-cpp/include/yaml-cpp)

add_subdirectory(yaml-cpp)

file(GLOB_RECURSE SRC "../../Common/Core/*.cpp" "../../Common/Core/*.hpp" "../../ThirdParty/logger/*.h" "../../ThirdParty/logger/*.cpp")

add_executable(ugm-cli main.cpp Commands.hpp Commands.cpp ${SRC})
set_target_properties(ugm-cli PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ugm-cli util pthread yaml-cpp)