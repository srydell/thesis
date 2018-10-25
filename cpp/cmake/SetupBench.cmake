# Don't build the tests for benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# Clang doesn't need anything
	set(BENCH_OPTIONS "")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# GCC need pthreads linked in
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)

	set(BENCH_OPTIONS Threads::Threads)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	find_library(SHLWAPI REQUIRED)
	set(BENCH_OPTIONS Threads::Threads)
endif()

# Define the 'benchmark' library
add_subdirectory(${PROJECT_SOURCE_DIR}/external/benchmark)

add_library(bench INTERFACE)
target_link_libraries(bench INTERFACE 
	benchmark
	${BENCH_OPTIONS}
)
