cmake_minimum_required (VERSION 3.8)

project ("DataManipulator")

add_executable (DataManipulator "DataManipulator.cpp" "DataManipulator.h" "DataFormats.h" "DataFormats.cpp" "WARNING.h" "WARNING.cpp" "DataReformer.h" "DataReformer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DataManipulator PROPERTY CXX_STANDARD 20)
endif()
