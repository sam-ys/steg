cmake_minimum_required(VERSION 3.0)
project(stegonography)

#
## Compilation options and source input
#######
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

string(TOLOWER ${CMAKE_BUILD_TYPE} MY_BUILD_TYPE)

if (MY_BUILD_TYPE STREQUAL "debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif (MY_BUILD_TYPE STREQUAL "debug")

if (MY_BUILD_TYPE STREQUAL "release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif (MY_BUILD_TYPE STREQUAL "release")

file(GLOB SRCS *.cpp)

#
## Install options
######
set(MY_APP_NAME steg)
add_executable(${MY_APP_NAME} ${SRCS})

set(CMAKE_INSTALL_PREFIX ../bin)
install(TARGETS ${MY_APP_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

#
## External libraries
#####
target_link_libraries(${MY_APP_NAME} LINK_PUBLIC gcrypt)
