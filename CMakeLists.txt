cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(mattres VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

add_library(${PROJECT_NAME} STATIC

src/Engine.cpp

src/util/get_files_from_folder.cpp
src/util/set_from_file_mtrs.cpp
src/util/mtrs_message.cpp
src/util/util_for_external_lib.cpp

# Systems
src/sys/SystemManager.cpp

src/sys/core/InputSystem.cpp
src/sys/core/ScriptsSystem.cpp
src/sys/core/UpdateTransformSystem.cpp

src/sys/rendering/CameraSystem.cpp
src/sys/rendering/SpriteRenderSystem.cpp
src/sys/rendering/AnimatorSystem.cpp
src/sys/rendering/StateAnimatorSystem.cpp
src/sys/rendering/SpriteMapRenderSystem.cpp
src/sys/rendering/MapAnimatorSystem.cpp

src/sys/audio/LocationAudioSystem.cpp

src/sys/ui/CursorFollowersSystem.cpp

# Resourses
src/res/ResourceManager.cpp

src/res/asset/ScriptFile.cpp
src/res/asset/RenderContext.cpp
src/res/asset/ShaderProgram.cpp
src/res/asset/Texture.cpp
src/res/asset/TextureAtlas.cpp
src/res/asset/Sound.cpp

src/res/render/SpriteBatch.cpp
src/res/render/BufferObject.cpp
src/res/render/VertexArrayObject.cpp

# Components

src/comp/ECSWorld.cpp

src/comp/collections/ComponentManager.cpp

src/comp/single/Window.cpp
src/comp/single/Camera.cpp
src/comp/single/States.cpp
src/comp/single/Cursor.cpp
src/comp/single/KeyButtons.cpp
src/comp/single/MouseButtons.cpp
src/comp/single/Audio.cpp
src/comp/single/Listener.cpp

src/comp/core/Script.cpp
src/comp/core/Transform.cpp

src/comp/rendering/Sprite.cpp
src/comp/rendering/Animator.cpp
src/comp/rendering/StateAnimator.cpp
src/comp/rendering/SpriteMap.cpp
src/comp/rendering/MapAnimator.cpp

src/comp/ui/CursorFollower.cpp

src/comp/audio/SoundPlayer.cpp

)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:FLAG_RELEASE>
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/mtrsstruct
    ${PROJECT_SOURCE_DIR}/external/single-files
)

add_subdirectory(external/glad glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(external/glfw glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(external/glm glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
