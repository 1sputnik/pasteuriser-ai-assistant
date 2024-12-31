cmake_minimum_required (VERSION 3.8)

project ("PAIA")

add_executable (PAIA "PAIA.cpp" "PAIA.h" "data_form.h" "data_form.cpp" "data_reform.h" "data_reform.cpp" "Scaler.h" "Scaler.cpp" "math_for_network.h" "math_for_network.cpp" "network.h" "network.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PAIA PROPERTY CXX_STANDARD 20)
endif()
