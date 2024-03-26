cmake_minimum_required (VERSION 3.8)

project ("DataManipulator")

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

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DataManipulator PROPERTY CXX_STANDARD 20)
endif()
