cmake_minimum_required(VERSION 3.28)
project(MMX VERSION 1.0 LANGUAGES CXX)

# rely on user-selected configuration for portability across generators:
# - single-config generators: pass -DCMAKE_BUILD_TYPE=Release (or Debug)
# - multi-config generators (Visual Studio, Xcode): choose config in IDE
# avoid hardcoding Debug for portability
# set default for single-config generators only when unspecified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

set(SFML_SKIP_INSTALL ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

add_executable(main 
    src/main.cpp
    src/menu.cpp
)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE 
    sfml-graphics 
    sfml-audio 
    sfml-network
)

add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)