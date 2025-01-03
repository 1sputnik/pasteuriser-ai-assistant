cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")

set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

project (PAIATests VERSION 2025.01.03.1)

set(PAIATests_SRC
        PAIATests.h
        PAIATests.cpp
   )

set(Common_SRC
        ../Common/DataFormats.h
        ../Common/DataFormats.cpp
        ../Common/WARNING.h
        ../Common/WARNING.cpp
        ../Common/DataReformer.h
        ../Common/DataReformer.cpp
        ../Common/DataReader.h
        ../Common/DataReader.cpp
        ../Common/DataDumper.h
        ../Common/DataDumper.cpp

        Common.Tests/DataFormats.Tests.h
        Common.Tests/DataFormats.Tests.cpp
    )

enable_testing()

add_subdirectory(Deps/googletest)

add_executable(PAIATests ${PAIATests_SRC} ${Common_SRC})

target_link_libraries(PAIATests gtest_main gmock_main)
