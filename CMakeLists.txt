cmake_minimum_required(VERSION 3.10)
project(SeaBattle CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходники в UTF-8 — иначе Visual Studio портит русский текст.
if(MSVC)
  add_compile_options(/utf-8)
endif()

include_directories(include)

# Тест поля — собирается сразу, без ИИ и интерфейса.
add_executable(board_test
  tests/board_test.cpp
  src/board.cpp
)

# Отдельная проверка компиляции ИИ.
add_library(ai_check OBJECT
  src/ai.cpp
)

# Игра собирается, когда в проекте появятся ai.cpp и display.cpp
# (после слияния веток feature/ai и feature/ui).
if(EXISTS ${CMAKE_SOURCE_DIR}/src/ai.cpp AND
   EXISTS ${CMAKE_SOURCE_DIR}/src/display.cpp)
  add_executable(seabattle
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/ai.cpp
    src/display.cpp
  )
else()
  message(STATUS "ai.cpp или display.cpp ещё нет — собирается только board_test")
endif()
