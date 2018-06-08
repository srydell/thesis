cmake_minimum_required(VERSION 3.8)
project(IsingWorm)

if(MSVC)
	add_compile_options(/W3 /WX)
else()
	add_compile_options(-W -Wall -Werror)
endif()

# Add all source files recursively
file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE HEADERS "source/*.h")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC source)
# Set C++ standard
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

# TODO: Fix this please, future Simon you are my only hope
# # This is how it should look
# # In each mitttest.cmake, include everything after -------------
# # include(tests/test1/mitttest.cmake)

# # -------------
# # Include testing library
# set(CATCH2_FOLDER external/catch2)
# set(TEST_NAME my_test)
# add_library(Catch INTERFACE)
# target_include_directories(Catch INTERFACE ${CATCH2_FOLDER})

# # Find all test files in test directory
# file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
# file(GLOB_RECURSE TEST_HEADERS "test/*.h")
# # Cannot use the globs from SOURCES & HEADERS since main.cpp has a main() function (Can only have one per executable)
# add_executable(${TEST_NAME} ${TEST_SOURCES} ${TEST_HEADERS} "source/Site.cpp" "source/Site.h" "source/Graph.h" "source/Graph.cpp" "source/IsingWorm.h" "source/IsingWorm.cpp")

# # Link to Catch
# target_link_libraries(${TEST_NAME} PRIVATE Catch)
# # Include from source directory
# target_include_directories(${TEST_NAME} PRIVATE source)
# # Set C++ standard
# target_compile_features(${TEST_NAME} PRIVATE cxx_std_14)

# # target_include_directories(${TEST_NAME} PUBLIC source)

# enable_testing()
# add_test(NAME Testing COMMAND ${TEST_NAME})

# # add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS my_test)
# add_dependencies(${TEST_NAME} ${PROJECT_NAME})
