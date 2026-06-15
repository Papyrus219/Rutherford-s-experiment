cmake_minimum_required(VERSION 3.10)
project(animation)

set(APP_SRC src/main.cpp
src/lib/app.cpp
glad/src/glad.c
src/lib/shader.cpp
src/lib/camera/camera.cpp
src/lib/camera/fly_camera.cpp
src/lib/camera/fps_camera.cpp
src/lib/camera/orbit_camera.cpp
src/lib/model/model.cpp
src/lib/model/mesh.cpp
src/lib/objects/molecule.cpp
src/lib/objects/vertecies.cpp)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)

add_executable(app ${APP_SRC})
target_include_directories(app PUBLIC include/pap_animation/ glad/include/)
target_compile_features(app PRIVATE cxx_std_23)

target_link_libraries(app PRIVATE OpenGL::OpenGL glfw glm::glm assimp Freetype::Freetype)

install(TARGETS app RUNTIME DESTINATION bin)

