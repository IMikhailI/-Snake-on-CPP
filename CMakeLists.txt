cmake_minimum_required(VERSION 3.10)
project(BrickGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# CLI sources
set(CLI_SOURCES
    brick_game/tetris/tetris_backend.c
    gui/cli/frontend.c
    gui/cli/tetris_cli.c
    gui/cli/snake_cli.c
    brick_game/common/common_backend.c
    brick_game/snake/backend.cpp
    brick_game/snake/snake_game.cpp
)

# GUI sources
set(GUI_SOURCES
    gui/desktop/game_widget.cpp
    gui/desktop/snake_window.cpp
    gui/desktop/tetris_game_widget.cpp
    gui/desktop/tetris_window.cpp
    gui/desktop/main_menu_window.cpp
)

set(MAIN_SRC main.cpp)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/brick_game/snake
    ${CMAKE_CURRENT_SOURCE_DIR}/brick_game/common
    ${CMAKE_CURRENT_SOURCE_DIR}/brick_game/tetris
)

add_executable(brickgame
    ${CLI_SOURCES}
    ${GUI_SOURCES}
    ${MAIN_SRC}
)

target_link_libraries(brickgame Qt5::Widgets ncurses)
target_compile_definitions(brickgame PRIVATE ENABLE_GUI) 

# Install target: установить бинарник в bin
install(TARGETS brickgame RUNTIME DESTINATION bin)

# Uninstall target (через скрипт)
if(NOT TARGET uninstall)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        IMMEDIATE @ONLY)
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif() 

# === GTest & Unit tests ===
include(CTest)
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_snake tests/test_snake.cpp brick_game/snake/snake_game.cpp brick_game/snake/backend.cpp brick_game/common/common_backend.c)
target_link_libraries(test_snake GTest::GTest GTest::Main pthread)
add_test(NAME test_snake COMMAND test_snake) 

add_executable(test_common tests/test_common.cpp brick_game/common/common_backend.c)
target_link_libraries(test_common GTest::GTest GTest::Main pthread)
add_test(NAME test_common COMMAND test_common) 

add_executable(test_tetris tests/test_tetris.cpp brick_game/tetris/tetris_backend.c brick_game/common/common_backend.c)
target_link_libraries(test_tetris GTest::GTest GTest::Main pthread)
add_test(NAME test_tetris COMMAND test_tetris) 