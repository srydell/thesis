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
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

# Include testing library
set(CATCH2_FOLDER external/catch2)
set(TEST_NAME tests)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH2_FOLDER})

# Find all test files in test directory
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
file(GLOB_RECURSE TEST_HEADERS "test/*.h")
add_executable(${TEST_NAME} ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(${TEST_NAME} Catch)

enable_testing()
add_test(NAME Testing COMMAND ${TEST_NAME})

# add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS my_test)
