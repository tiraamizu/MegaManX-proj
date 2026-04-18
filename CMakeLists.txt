cmake_minimum_required(VERSION 3.28)
project(MMX VERSION 1.0 LANGUAGES CXX)

# Default build type for single-config generators
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output exe to /bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

set(SFML_SKIP_INSTALL ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

# Your executable
add_executable(main 
    src/main.cpp
)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE 
    sfml-graphics 
    sfml-audio 
    sfml-network
)

# ✅ FIXED run target (important change here)
add_custom_target(run
    COMMAND $<TARGET_FILE:main>
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Set startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)