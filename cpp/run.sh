#!/usr/bin/env bash

# Make sure that build directory is available
if ! [[ -d build ]]; then
	mkdir build
fi

function run_project {
	cd ./build || exit

	# Run binary build succeeds
	if make --no-print-directory "$1"; then
		time ./IsingWorm ;
	fi

	cd - || exit
}

if [[ "$1" == build ]]; then
	# Build the makefiles using g++
	#+Clang seem to not like some of the design choices of Catch2
	compiler=$(command -v g++)
	cmake -DCMAKE_CXX_COMPILER="$compiler" -DCMAKE_INSTALL_PREFIX="/home/bin/gcc" --config Release --build ./build -H.

	# Regenerate the doxygen documentation
	doxygen &> /dev/null

	# Run the code
	# run_project all
	exit
elif [[ "$1" == test ]]; then
	cd ./build || exit

	# What it is called in the CMakeLists.txt
	custom_test_name=Testing
	# Recompile everything if necessary
	if make $custom_test_name; then
		# Run the tests
		# ctest --output-on-failure $custom_test_name
		./$custom_test_name
	fi
else
	# On clean make, just run the project
	run_project IsingWorm
fi
