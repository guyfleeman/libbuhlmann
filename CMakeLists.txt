cmake_minimum_required(VERSION 3.10)
project(libbuhlman VERSION 0.0.1 DESCRIPTION "an implementation of the Buhlmann ZHL algorithms")

set(CMAKE_CXX_STANDARD 11)

#############################################################################
#  DSO CONFIG
#############################################################################

include_directories(include)
include_directories(include/ec)
include_directories(include/model)
include_directories(include/planning)
include_directories(include/schedule)
include_directories(include/units)

set(LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(APP_SRC_DIR ${PROJECT_SOURCE_DIR}/app)

file(GLOB_RECURSE LIB_SOURCES
        ${LIB_SRC_DIR}/*.cpp
        ${LIB_SRC_DIR}/model/*.cpp
        ${LIB_SRC_DIR}/planning/*.cpp)

file(GLOB_RECURSE LIB_HEADERS include/*.h include/*.hpp)

add_library(libbuhlmann SHARED ${LIB_SOURCES} ${LIB_HEADERS})

set_target_properties(libbuhlmann PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libbuhlmann PROPERTIES SOVERSION 0)
set_target_properties(libbuhlmann PROPERTIES PUBLIC_HEADER include/mylib.h)

##################################################################################################
# TEST PLANNING
##################################################################################################

target_include_directories(libbuhlmann PRIVATE ${HDRS})
file(GLOB_RECURSE TEST_BUHLMANN_HEADERS include/*.h include/*.hpp)

add_executable(planning_testing app/main_test_planning.cpp)
target_link_libraries(planning_testing libbuhlmann)

##################################################################################################
# TEST BUHLMANN
##################################################################################################

target_include_directories(libbuhlmann PRIVATE ${HDRS})
file(GLOB_RECURSE TEST_PLANNING_HEADERS include/*.h include/*.hpp)

add_executable(buhlmann_testing app/main_test_buhlmann.cpp)
target_link_libraries(buhlmann_testing libbuhlmann)

include(GNUInstallDirs)
