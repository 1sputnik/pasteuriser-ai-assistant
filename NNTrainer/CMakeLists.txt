cmake_minimum_required (VERSION 3.8)

project ("NNTrainer")

set(SOURCE_FILES
        NNTrainer.h
        NNTrainer.cpp
        ModelCreater.h
        ModelCreater.cpp
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
        ../NeuroModels/NeuroSkeletons.h
        ../NeuroModels/NeuroSkeletons.cpp
        ../NeuroModels/ActFunctions.h
        ../NeuroModels/ActFunctions.cpp
        ../NeuroModels/Scaler.h
        ../NeuroModels/Scaler.cpp
        ../NeuroModels/WeightFiller.h
        ../NeuroModels/WeightFiller.cpp
        ../NeuroModels/VanilaLSTM.h
        ../NeuroModels/VanilaLSTM.cpp
        ../NeuroModels/TInVanilaLSTM.h
        ../NeuroModels/TInVanilaLSTM.cpp
        )

add_executable (NNTrainer ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NNTrainer PROPERTY CXX_STANDARD 20)
endif()
