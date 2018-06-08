#!/usr/bin/env bash

cd ./build || exit

function run_project {
	# Run binary build succeeds
	if make --no-print-directory "$1"; then
		time ./IsingWorm ;
	fi
}

if [[ "$1" == build ]]; then
	# Build the makefiles using g++
	#+Clang seem to not like some of the design choices of Catch2
	cmake -DCMAKE_CXX_COMPILER="$(which g++)" ../

	# Regenerate the doxygen documentation
	cd ../ || exit
	doxygen &> /dev/null
	cd ./build || exit

	# Run the code
	run_project all
	exit
elif [[ "$1" == test ]]; then
	# run_project test

	# What it is called in the CMakeLists.txt
	custom_test_name=my_test
	# Recompile everything if necessary
	make $custom_test_name
	# Run the tests
	ctest --output-on-failure $custom_test_name
else
	# On clean make, just run the project
	run_project IsingWorm
fi
