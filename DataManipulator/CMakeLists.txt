cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)

project(DataManipulator VERSION 2025.01.20.1)

file(GLOB DM_Sources CONFIGURE_DEPENDS
    ./*.cpp
    ./*.h
)

file(GLOB Common_Sources CONFIGURE_DEPENDS
    ../Common/*.cpp
    ../Common/*.h
)

add_executable (DataManipulator ${Common_Sources} ${DM_Sources})
