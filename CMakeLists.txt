cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")

set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

project (PAIA VERSION 2024.12.31.1)

add_subdirectory(DataManipulator)

add_subdirectory(NNTrainer)

set(PAIA_SRC
        paia.h
        paia.cpp
   )

enable_testing()

add_subdirectory(deps/googletest)

add_executable(PAIA ${PAIA_SRC})

target_link_libraries(PAIA gtest_main gmock_main)
