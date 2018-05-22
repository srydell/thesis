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
