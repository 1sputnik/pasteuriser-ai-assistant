cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)

project (DataManipulator VERSION 2025.01.09.2)

set(DM_FILES
        DataManipulator.h
        DataManipulator.cpp
        MainOCDFMenu.h
        MainOCDFMenu.cpp
        MainTDFMenu.h
        MainTDFMenu.cpp
        TDFCreator.h
        TDFCreator.cpp
        DMIncludes.h
)

file(GLOB Common_Files CONFIGURE_DEPENDS
    	    ../Common/*.cpp
            ../Common/*.h
)

add_executable (DataManipulator ${Common_Files} ${DM_FILES})

