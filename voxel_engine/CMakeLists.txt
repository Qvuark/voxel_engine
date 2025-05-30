﻿cmake_minimum_required(VERSION 3.10)
project(voxel_engine)

set(CMAKE_CXX_STANDARD 17)

# Добавляем исходники и создаем исполняемый файл
add_executable(voxel_engine 
	"voxel_engine.cpp" 
	"window/window.cpp" 
	"window/events.cpp" 
	"graphics/shaders.cpp"
	"graphics/textures.cpp"
	"loaders/png_loading.cpp"
	"window/camera.cpp"

	"window/window.h" 
	"voxel_engine.h"
	"window/events.h"   
 	"graphics/shaders.h"
	"graphics/textures.h" 
	"loaders/png_loading.h"
	"window/camera.h"
 "graphics/mesh.cpp" "graphics/mesh.h" "voxels/voxels.h" "voxels/voxels.cpp" "voxels/chunk.cpp" "graphics/voxel_renderer.cpp" 
 "graphics/voxel_renderer.h" "voxels/chunks.cpp" "voxels/chunks.h"  "blocks/AirBlock.h"  
 "blocks/WaterBlock.h" "blocks/DirtBlock.h"   "blocks/AndesiteBlock.h" "blocks/CobblestoneBlock.h" 
  "blocks/SlaneBlock.h"  "blocks/CoalBlock.h"  "blocks/BedrockBlock.h"  
 "blocks/WoodBlock.h"  "blocks/LeavesBlock.h"  "blocks/BlockRegister.h"  "blocks/PlayersBlock.h"  )

# Находим GLFW, установленную через vcpkg
find_package(glfw3  CONFIG REQUIRED)
find_package(OpenGL REQUIRED MODULE)
find_package(GLEW   CONFIG REQUIRED)
find_package(PNG    CONFIG REQUIRED)
find_package(ZLIB          REQUIRED)
find_package(glm           REQUIRED)

# Привязываем GLFW и OpenGL к нашей цели
target_link_libraries(voxel_engine PRIVATE 
	glfw 	
	OpenGL::GL 
	GLEW::GLEW
	PNG::PNG
	ZLIB::ZLIB

)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET voxel_engine PROPERTY CXX_STANDARD 17)
endif()
