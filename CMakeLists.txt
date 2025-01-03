cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)

project (PAIA VERSION 2025.01.03.1)

add_subdirectory(DataManipulator)

add_subdirectory(NNTrainer)

add_subdirectory(Tests)

set(PAIA_SRC
        paia.h
        paia.cpp
   )

add_executable(PAIA ${PAIA_SRC})
