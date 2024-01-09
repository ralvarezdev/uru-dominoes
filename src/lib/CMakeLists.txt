add_library(lib STATIC
cards.cpp
cards.h
input.cpp
input.h
terminal.cpp
terminal.h)

target_include_directories(lib PRIVATE ${CMAKE_CURRENT_LIST_DIR})