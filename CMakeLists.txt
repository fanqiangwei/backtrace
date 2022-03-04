###################################
# Test 
###################################
cmake_minimum_required(VERSION 3.5)

project(TEST)
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
include_directories(
./
./diagnose/
)

set(Test_SRC
        main.cpp
        callback.cpp
        diagnose/diagnose.cpp
)
add_definitions(-std=c++11)      
add_executable(${PROJECT_NAME} ${Test_SRC})
