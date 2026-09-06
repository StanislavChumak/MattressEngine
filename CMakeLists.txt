cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(MattressEngine)
set(LIB_NAME "mattress")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

if (WIN32)
    set_target_properties(${LIB_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

add_subdirectory(external/utils mtrsutils)
add_subdirectory(external/glad glad)
add_subdirectory(external/glfw glfw)
add_subdirectory(external/glm glm)

add_library(${LIB_NAME} STATIC

src/Engine.cpp

# Systems
src/sys/SystemManager.cpp

src/sys/core/InputSystem.cpp
src/sys/core/InputSybscrubersSystem.cpp
src/sys/core/ScriptsSystem.cpp
src/sys/core/TransformSystem.cpp

src/sys/rendering/CameraSystem.cpp
src/sys/rendering/SpriteSubmitSystem.cpp
src/sys/rendering/AnimatorSystem.cpp
src/sys/rendering/StateAnimatorSystem.cpp
src/sys/rendering/SpriteMapSubmitSystem.cpp
src/sys/rendering/MapAnimatorSystem.cpp
src/sys/rendering/TextSubmitSystem.cpp
src/sys/rendering/RenderSystem.cpp

src/sys/audio/LocationAudioSystem.cpp

# Resourses
src/res/ResourceManager.cpp

src/res/asset/ScriptFile.cpp
src/res/asset/ShaderProgram.cpp
src/res/asset/Texture.cpp
src/res/asset/TextureAtlas.cpp
src/res/asset/Sound.cpp
src/res/asset/Font.cpp

src/res/render/SpriteBatch.cpp
src/res/render/BufferObject.cpp
src/res/render/VertexArrayObject.cpp

# Components
src/comp/ECSWorld.cpp

src/comp/collections/ComponentManager.cpp

src/comp/single/Window.cpp
src/comp/single/Camera.cpp
src/comp/single/Render.cpp
src/comp/single/GlyphDecoder.cpp
src/comp/single/States.cpp
src/comp/single/Cursor.cpp
src/comp/single/KeyButtons.cpp
src/comp/single/MouseButtons.cpp
src/comp/single/MouseScroll.cpp
src/comp/single/Audio.cpp
src/comp/single/Listener.cpp

src/comp/core/ScriptUpdate.cpp
src/comp/core/ScriptCallback.cpp
src/comp/core/StoredData.cpp
src/comp/core/Transform.cpp

src/comp/rendering/Sprite.cpp
src/comp/rendering/Animator.cpp
src/comp/rendering/StateAnimator.cpp
src/comp/rendering/SpriteMap.cpp
src/comp/rendering/MapAnimator.cpp

src/comp/ui/Label.cpp

src/comp/audio/SoundPlayer.cpp

)

set_property(TARGET ${LIB_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

target_compile_definitions(${LIB_NAME} PRIVATE
    $<$<CONFIG:Release>:FLAG_RELEASE>
)

target_include_directories(${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/mtrsstruct
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/single-files
)

target_link_libraries(${LIB_NAME} PUBLIC mtrsutils)
target_link_libraries(${LIB_NAME} PUBLIC glad)
target_link_libraries(${LIB_NAME} PUBLIC glfw)
target_link_libraries(${LIB_NAME} PUBLIC glm)
