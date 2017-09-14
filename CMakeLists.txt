cmake_minimum_required(VERSION 3.7)
project(GA_Trusser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(X:/Eigen/Eigen)

find_package(SFML 2 REQUIRED system graphics window)
include_directories(${SFML_INCLUDE_DIR})

set(SOURCE_FILES main.cpp
        Genotype.cpp Genotype.h
        EvolutionParameters.cpp EvolutionParameters.h
        Global.h
        Hyperparameters.cpp Hyperparameters.h
        Vivarium.cpp Vivarium.h
        Evolver.cpp Evolver.h
        Point.h Species.cpp Species.h)
add_executable(GA_Trusser ${SOURCE_FILES})
target_link_libraries(GA_Trusser ${SFML_LIBRARIES} ${SFML_DEPENDECIES})