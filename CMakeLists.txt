cmake_minimum_required(VERSION 3.0)

project(netcube)

add_executable(netcube main.cpp src/Texture.cpp src/Shaders.cpp src/ObjectLoader.cpp src/Controls.cpp src/Network.cpp)

find_package(Boost COMPONENTS thread system REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(fmt REQUIRED)
find_package(PNG REQUIRED)

include_directories(${PNG_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

target_link_libraries(netcube ${PNG_LIBRARY} ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} ${Boost_LIBRARIES} fmt::fmt)

install(TARGETS netcube RUNTIME DESTINATION bin)
