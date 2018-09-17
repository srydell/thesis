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

# Add custom scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Set warning flags for Windows and Unix
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Wall -Werror)
endif()

include(SetupLibraries)
include(SetupTests)

# Set IsingWorm executable
add_executable(IsingWorm application/isingmain.cpp)
target_include_directories(IsingWorm PRIVATE source source/Ising)
target_link_libraries(IsingWorm PRIVATE IsingGraph)

# Set xyWorm executable
add_executable(xyWorm application/xymain.cpp)
target_include_directories(xyWorm PRIVATE source source/xy)
target_link_libraries(xyWorm PRIVATE XYGraph)
