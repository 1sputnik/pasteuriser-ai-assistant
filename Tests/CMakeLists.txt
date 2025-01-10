cmake_minimum_required (VERSION 3.0) # задаём версию CMake

set(CMAKE_CXX_STANDARD 23) # устанавливаем стандарт языка С++

set(CMAKE_CXX_FLAGS_RELEASE "/MT") # устанавливаем MultiThread сборку для релиза (чтобы работали submodules)

set(CMAKE_CXX_FLAGS_DEBUG "/MTd") # устанавливаем MultiThread сборку для дебагера (чтобы работали submodules)

project (PAIATests VERSION 2025.01.09.1) # создаём проект и его версию

# указываем файлы, которые будут подключены к проекту
file(GLOB PAIATests_SRC CONFIGURE_DEPENDS
        PAIATests.h
        PAIATests.cpp
        includes.h
    )

file(GLOB Common_SRC CONFIGURE_DEPENDS
        Common.Tests/*.h
        Common.Tests/*.cpp

        ../Common/*.cpp
        ../Common/*.h
    )

enable_testing() # "включаем" тесты

add_subdirectory(Deps/googletest) # подключаем submodule googletest

set(SUBHOOK_TESTS OFF) # отключаем тесты у submodule subhook

add_subdirectory(deps/subhook EXCLUDE_FROM_ALL) # подключаем submodule subhook

set_target_properties (subhook gtest gtest_main gmock gmock_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}) # собираем все dll в той же директории, что и exe

add_executable(PAIATests ${PAIATests_SRC} ${Common_SRC}) # инициализируем проект

target_link_libraries(PAIATests gtest_main gmock_main) # подключаем библиотеки фрейворка googletest, чтобы не прописывать пути вручную

target_link_libraries(PAIATests subhook) # подключаем библиотеки фрейворка subhook, чтобы не прописывать пути вручную

# задаём список файлов, которые нужны для тестирования 
set(FILES_FOR_TESTS
    TestFiles/Corrupted.ocdf.csv
    TestFiles/Holey.ocdf.csv
    TestFiles/One_Line.ocdf.csv
    TestFiles/One_Line.tdf.csv
    TestFiles/Void.csv
    TestFiles/Invalid.csv
    TestFiles/Rigth.ocdf.csv
    TestFiles/Rigth.ocdf.bin
    TestFiles/TestData.ocdf.bin
    TestFiles/Rigth.tdf.bin
    TestFiles/Rigth.tdf.csv
    TestFiles/TestData.tdf.bin
    )

# копируем все файлы с данными и моделями нейронных сетей в папку с exe
foreach(file_name IN LISTS FILES_FOR_TESTS)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file_name} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()