cmake_minimum_required (VERSION 3.8) # задаём версию CMake

set(CMAKE_CXX_STANDARD 23) # устанавливаем стандарт языка С++

set(CMAKE_CXX_FLAGS_RELEASE "/MT") # устанавливаем MultiThread сборку для релиза (чтобы работали submodules)

set(CMAKE_CXX_FLAGS_DEBUG "/MTd") # устанавливаем MultiThread сборку для дебагера (чтобы работали submodules)

project (PAIATests VERSION 2025.01.03.1) # создаём проект и его версию

# указываем файлы, которые будут подключены к проекту
set(PAIATests_SRC
        PAIATests.h
        PAIATests.cpp
        includes.h
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
        Common.Tests/DataReader.Tests.h
        Common.Tests/DataReader.Tests.cpp
        Common.Tests/Warning.Tests.h
        Common.Tests/Warning.Tests.cpp
    )

enable_testing() # "включаем" тесты

add_subdirectory(Deps/googletest) # подключаем submodule googletest

set(SUBHOOK_TESTS OFF) # отключаем тесты у submodule subhook

add_subdirectory(deps/subhook EXCLUDE_FROM_ALL) # подключаем submodule subhook

set_target_properties (subhook gtest gtest_main gmock gmock_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}) # собираем все dll в той же директории, что и exe

add_executable(PAIATests ${PAIATests_SRC} ${Common_SRC}) # инициализируем проект

target_link_libraries(PAIATests gtest_main gmock_main) # подключаем библиотеки фрейворка googletest, чтобы не прописывать пути вручную

target_link_libraries(PAIATests subhook) # подключаем библиотеки фрейворка subhook, чтобы не прописывать пути вручную

