cmake_minimum_required(VERSION 3.15)
project(GameProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "C:/SFML")
list(APPEND CMAKE_PREFIX_PATH "${SFML_ROOT}")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(GameProject
        main.cpp
        Bird.cpp
        Bird.h
        GameConstants.h
        GameWindow.cpp
        GameWindow.h
        Menu.cpp
        Menu.h
        Obstacles.cpp
        Obstacles.h
        RankingList.cpp
        RankingList.h
        Score.cpp
        Score.h
        User.cpp
        User.h
        Vortex.cpp
        Vortex.h
        sqlite/sqlite3.c
        sqlite/sqlite3.h
        PowerUp.cpp
        PowerUp.h
)

target_include_directories(GameProject PRIVATE "C:/SFML/include")

target_link_libraries(GameProject PRIVATE sfml-graphics sfml-window sfml-system)

file(GLOB SFML_DLLS "${SFML_ROOT}/bin/*.dll")

add_custom_command(TARGET GameProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLLS}
        $<TARGET_FILE_DIR:GameProject>
)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")