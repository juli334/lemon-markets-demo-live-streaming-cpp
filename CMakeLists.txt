### Set up the project ###

cmake_minimum_required(VERSION 3.7)
project(stream)

# Set the default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


### Create the executable ###

# Mark this project as an executable and add all source files to it for compilation
add_executable(
    ${PROJECT_NAME}
    json.hpp
    main.cpp
)


### Link system libraries ###

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    curl
    pthread
    paho-mqtt3cs
    #paho-mqttpp3
)