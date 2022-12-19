cmake_minimum_required(VERSION 3.23)

project(Snake)

set(CMAKE_CXX_STANDARD 14)

add_executable(Snake
    Worm.rc
        src/states/game/Coord.hpp
        src/states/DeathState.cpp src/states/DeathState.hpp
        src/Game.cpp src/Game.hpp
        src/states/GameState.hpp
        src/utils/Font.hpp
        src/states/menu-states/MainMenuState.cpp src/states/menu-states/MainMenuState.hpp
        src/states/menu-states/MenuState.cpp src/states/menu-states/MenuState.hpp
        src/states/menu-states/PauseState.cpp src/states/menu-states/PauseState.hpp
        src/states/PlayingState.cpp src/states/PlayingState.hpp
        src/states/game/Score.hpp
        src/states/menu-states/SkinMenuState.cpp src/states/menu-states/SkinMenuState.hpp
        src/states/game/Snake.cpp src/states/game/Snake.hpp
        src/Source.cpp
        src/utils/Texture.cpp src/utils/Texture.hpp)

target_include_directories(Snake PUBLIC D:/Konstantinos/Programming/libraries/include/SDL2)
target_link_directories(Snake PUBLIC D:/Konstantinos/Programming/libraries/lib)

target_link_libraries(Snake SDL2main SDL2 SDL2_image SDL2_ttf)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET Snake PROPERTY WIN32_EXECUTABLE true)
  set_target_properties(Snake PROPERTIES OUTPUT_NAME "Snake-release")
endif()