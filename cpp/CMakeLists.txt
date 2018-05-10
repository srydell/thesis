cmake_minimum_required(VERSION 3.8)
project(IsingWorm)

if(MSVC)
	add_compile_options(/W3 /WX)
else()
	add_compile_options(-W -Wall -Werror)
endif()

add_executable(IsingWorm main.cpp)

# Set C++ standard
target_compile_features(IsingWorm PRIVATE cxx_std_14)

