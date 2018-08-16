cmake_minimum_required(VERSION 3.8)
# Languages CXX tells cmake to not try to compile with C compilers etc
project(MasterThesis LANGUAGES CXX)

# Set C++ 14 globally
set(CMAKE_CXX_STANDARD 14)
# Possible to set it target specific as such:
# set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

# Fail if the set C++ standard is not available
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Do not use GNU extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile flags to compile_commands.json database.
# Useful for linters and autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set warning flags for Windows and Unix
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Wall -Werror)
endif()

add_library(Graph
	source/Graph.cpp
	source/Graph.h
	source/IndexCluster.cpp
	source/HKIndex.cpp
	source/Site.cpp
	source/Site.h
	source/Percolation.cpp
	source/ClusterDimension.cpp
	source/utils.h
)
target_include_directories(Graph PRIVATE source)

add_library(Ising
	source/IsingWorm.cpp
	source/IsingWorm.h
)
target_include_directories(Ising PRIVATE source)

# Set IsingWorm executable
add_executable(IsingWorm application/main.cpp)
target_include_directories(IsingWorm PRIVATE source)
target_link_libraries(IsingWorm PRIVATE Graph)
target_link_libraries(IsingWorm PRIVATE Ising)

# Set testing of source files
set(CATCH_DIR external/catch2)
add_executable(Testing ${CATCH_DIR}/catch.hpp source/test/test1.cpp)
target_include_directories(Testing PRIVATE source)

# Define Catch as a library
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_DIR})
target_link_libraries(Testing PRIVATE Catch)
target_link_libraries(Testing PRIVATE Graph)
target_link_libraries(Testing PRIVATE Ising)

enable_testing()

# Add test to ctest
#             Name            HowToRun
add_test(NAME Testing COMMAND Testing)
