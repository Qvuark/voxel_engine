cmake_minimum_required(VERSION 3.10)
project(voxel_engine)

# Добавляем исходники и создаем исполняемый файл
add_executable(voxel_engine 
	"voxel_engine.cpp" 
	"voxel_engine.h" 
	"window/window.h" 
	"window/window.cpp" 
	"window/events.cpp" 
	"window/events.h")

# Находим GLFW, установленную через vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED MODULE)
find_package(GLEW CONFIG REQUIRED)

# Привязываем GLFW и OpenGL к нашей цели
target_link_libraries(voxel_engine PRIVATE glfw OpenGL::GL GLEW::GLEW)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET voxel_engine PROPERTY CXX_STANDARD 20)
endif()
