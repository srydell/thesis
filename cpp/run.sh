#!/usr/bin/env bash

cd ./build || exit

function run_project {
	# Run binary build succeeds
	# if cmake --build .; then
	if make --no-print-directory "$1"; then
		time ./IsingWorm ;
	fi
}

if [[ "$1" == build ]]; then
	# Build the makefiles
	cmake ../

	# Regenerate the doxygen documentation
	cd ../ || exit
	doxygen &> /dev/null
	cd ./build || exit

	# Run the code
	run_project all
	exit
elif [[ "$1" == test ]]; then
	# run_project test

	# Recompile everything if necessary
	make tests &> /dev/null
	# Run the tests
	ctest --output-on-failure
else
	# On clean make, just run the project
	run_project IsingWorm
fi
