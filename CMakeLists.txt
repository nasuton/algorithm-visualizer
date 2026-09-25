cmake_minimum_required(VERSION 3.28)

project(AlgorithmVisualization VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Static linking avoids having to distribute SFML DLLs by default.
option(BUILD_SHARED_LIBS "Build dependency libraries as shared libraries" OFF)

include(FetchContent)

# Only the graphics, window and system modules are needed for visualization.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML network" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "Build SFML graphics" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "Build SFML window" FORCE)
set(SFML_CONFIGURE_EXTRAS OFF CACHE BOOL "Configure SFML extras" FORCE)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_library(sorting STATIC
    src/BubbleSort.cpp
    src/MergeSort.cpp
    src/InsertionSort.cpp
    src/SelectionSort.cpp
    src/QuickSort.cpp
    src/HeapSort.cpp
    src/CountingSort.cpp
    src/RadixSort.cpp
)
target_include_directories(sorting PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(sorting PUBLIC cxx_std_17)
set_target_properties(sorting PROPERTIES CXX_EXTENSIONS OFF)

add_executable(algorithm_visualization src/main.cpp)
target_compile_features(algorithm_visualization PRIVATE cxx_std_17)
set_target_properties(algorithm_visualization PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(algorithm_visualization PRIVATE sorting SFML::Graphics)

if(MSVC)
    target_compile_options(algorithm_visualization PRIVATE /W4 /utf-8)
    target_compile_options(sorting PRIVATE /W4 /utf-8)
else()
    target_compile_options(algorithm_visualization PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sorting PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Also support users opting into shared libraries on Windows.
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET algorithm_visualization POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:algorithm_visualization>
            $<TARGET_FILE_DIR:algorithm_visualization>
        COMMAND_EXPAND_LISTS
        VERBATIM
    )
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT algorithm_visualization)

include(CTest)
if(BUILD_TESTING)
    add_executable(sorting_tests tests/sorting_tests.cpp)
    target_link_libraries(sorting_tests PRIVATE sorting)
    add_test(NAME sorting_tests COMMAND sorting_tests)
endif()
