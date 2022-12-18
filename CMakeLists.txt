cmake_minimum_required(VERSION 3.23)

project(Snake)

set(CMAKE_CXX_STANDARD 14)

add_executable(Snake
    Worm.rc
    coord.hpp
    DeathState.cpp DeathState.hpp
    Game.cpp Game.hpp
    GameState.hpp
    Font.hpp
    MainMenuState.cpp MainMenuState.hpp
    MenuState.cpp MenuState.hpp
    PauseState.cpp PauseState.hpp
    PlayingState.cpp PlayingState.hpp
    Score.hpp
    SkinMenuState.cpp SkinMenuState.hpp
    Snake.cpp Snake.hpp
    Source.cpp
    Texture.cpp Texture.hpp)

target_include_directories(Snake PUBLIC D:/Konstantinos/Programming/libraries/include/SDL2)
target_link_directories(Snake PUBLIC D:/Konstantinos/Programming/libraries/lib)

target_link_libraries(Snake SDL2main SDL2 SDL2_image SDL2_ttf)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET Snake PROPERTY WIN32_EXECUTABLE true)
  set_target_properties(Snake PROPERTIES OUTPUT_NAME "Snake-release")
endif()