cmake_minimum_required(VERSION 3.23)

project(Snake)

set(CMAKE_CXX_STANDARD 14)

add_executable(Snake Worm.rc Source.cpp Game.cpp Game.hpp SnakeGame.cpp SnakeGame.hpp Snake.cpp Snake.hpp Texture.cpp Texture.hpp Score.hpp coord.hpp)

target_include_directories(Snake PUBLIC D:/Konstantinos/Programming/libraries/include/SDL2)
target_link_directories(Snake PUBLIC D:/Konstantinos/Programming/libraries/lib)

target_link_libraries(Snake SDL2main SDL2 SDL2_image SDL2_ttf)