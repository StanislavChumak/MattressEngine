cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(PROJECT_NAME 2048)
project(${PROJECT_NAME} VERSION 0.1.0)


#add_compile_definitions(FLAG_RELEASE)


add_executable(${PROJECT_NAME} 
src/main.cpp
src/system/init.cpp

external/stb_image.h
external/miniaudio.h

src/system/Timer.h
src/system/Timer.cpp
src/system/String.h
src/system/String.cpp
src/system/Soloptr.h

src/renderer/ShaderProgram.h
src/renderer/ShaderProgram.cpp
src/renderer/Texture2D.h
src/renderer/Texture2D.cpp
src/renderer/Sprite2D.h
src/renderer/Sprite2D.cpp
src/renderer/Sprite2DAnimator.h
src/renderer/Sprite2DAnimator.cpp
src/renderer/BufferObject.h
src/renderer/BufferObject.cpp
src/renderer/VertexArrayObject.h
src/renderer/VertexArrayObject.cpp
src/renderer/Renderer.h
src/renderer/Renderer.cpp

src/sound/Sound.h
src/sound/Sound.cpp

src/resources/ResourceManager.h
src/resources/ResourceManager.cpp

src/game/Game.h
src/game/Game.cpp

src/game/game_states/GameState.h
src/game/game_states/StartScreen.h
src/game/game_states/StartScreen.cpp
src/game/game_states/GamePlay.h
src/game/game_states/GamePlay.cpp

src/game/game_objects/Button.h
src/game/game_objects/Button.cpp
)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory (external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)
# include_directories(external/glm)

include_directories(external/rapidjson/include)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/res 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
