cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)

project (DataManipulator VERSION 2024.12.31.1)

set(SOURCE_FILES
        DataManipulator.h
        DataManipulator.cpp
        MainOCDFMenu.h
        MainOCDFMenu.cpp
        MainTDFMenu.h
        MainTDFMenu.cpp
        TDFCreator.h
        TDFCreator.cpp
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
        )

add_executable (DataManipulator ${SOURCE_FILES})

