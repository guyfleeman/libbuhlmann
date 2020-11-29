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

file(GLOB_RECURSE LIB_SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/model/*.cpp
        ${PROJECT_SOURCE_DIR}/planning/*.cpp)

file(GLOB_RECURSE LIB_HEADERS include/*.h include/*.hpp)

add_library(libbuhlmann SHARED ${LIB_SOURCES} ${LIB_HEADERS})

set_target_properties(libbuhlmann PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libbuhlmann PROPERTIES SOVERSION 0)
set_target_properties(libbuhlmann PROPERTIES PUBLIC_HEADER include/mylib.h)

##################################################################################################
# TEST PLANNING
##################################################################################################

target_include_directories(libbuhlmann PRIVATE ${HDRS})
target_include_directories(libbuhlmann PRIVATE ${SRCS})

file(GLOB_RECURSE TEST_PLANNING_SOURCES app/main_test_planning.cpp)
file(GLOB_RECURSE TEST_PLANNING_HEADERS include/*.h include/*.hpp)

add_executable(planning_testing ${TEST_PLANNING_SOURCES})
target_link_libraries(planning_testing libbuhlmann)



include(GNUInstallDirs)
