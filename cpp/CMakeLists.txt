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
include(SetupBench)

# Set IsingWorm executable
add_executable(IsingWorm application/isingmain.cpp)
target_include_directories(IsingWorm PRIVATE source source/Ising)
target_link_libraries(IsingWorm PRIVATE IsingGraph)

# Set xyWorm executable
add_executable(xyWorm application/xymain.cpp)
target_include_directories(xyWorm PRIVATE source source/xy)
target_link_libraries(xyWorm PRIVATE XYGraph)

# Set closeToTc executable
add_executable(closeToTc ./application/closetotcxy.cpp)
target_include_directories(closeToTc PRIVATE source source/xy)
target_link_libraries(closeToTc PRIVATE XYGraph)

# Set warmup executable
add_executable(warmup application/warmup.cpp)
target_include_directories(warmup PRIVATE source source/xy)
target_link_libraries(warmup PRIVATE XYGraph)

# Set energy_scaling executable
add_executable(energy_scaling application/energy_scaling_xy.cpp)
target_include_directories(energy_scaling PRIVATE source source/xy)
target_link_libraries(energy_scaling PRIVATE XYGraph)

# Set measure_graphdiv executable
add_executable(measure_graphdiv application/measure_graphdiv.cpp)
target_include_directories(measure_graphdiv PRIVATE source source/xy)
target_link_libraries(measure_graphdiv PRIVATE XYGraph)

# Set convert_warmups executable
# Read the xyl*t*.txt files and calulate energy/winding number and rewrite them
add_executable(convert_warmups application/convert_warmups.cpp)
target_include_directories(convert_warmups PRIVATE source source/xy)
target_link_libraries(convert_warmups PRIVATE XYGraph)

# Set gen_frames executable
# Generates a set of xy graph files which will be used for animation
add_executable(gen_frames application/gen_frames.cpp)
target_include_directories(gen_frames PRIVATE source source/xy)
target_link_libraries(gen_frames PRIVATE XYGraph)

# Set gen_frames_ising executable
# Generates a set of xy graph files which will be used for animation
add_executable(gen_frames_ising application/gen_frames_ising.cpp)
target_include_directories(gen_frames_ising PRIVATE source source/Ising)
target_link_libraries(gen_frames_ising PRIVATE IsingGraph)

# Set gen_large_cluster_ising executable
# Generates a set of xy graph files which will be used for animation
add_executable(gen_large_cluster_ising application/gen_large_cluster_ising.cpp)
target_include_directories(gen_large_cluster_ising PRIVATE source source/Ising)
target_link_libraries(gen_large_cluster_ising PRIVATE IsingGraph)

# Set testbench executable
# Generates a set of xy graph files which will be used for animation
add_executable(graph-bench source/benchmark/bench_divgraph.cpp)
target_include_directories(graph-bench PRIVATE source source/xy)
target_link_libraries(graph-bench PRIVATE bench)
target_link_libraries(graph-bench PRIVATE XYGraph)
