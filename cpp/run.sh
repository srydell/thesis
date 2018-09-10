#!/usr/bin/env bash

# Make sure that build directory is available
if ! [[ -d build ]]; then
	mkdir build
fi

function run_project {
	# Regenerate the doxygen documentation
	doxygen &> /dev/null

	# Run binary build succeeds
	if cmake --build build --target "$1"; then
		cd ./build || exit
		time ./"$1" ;
		cd - || exit
	fi
}

if [[ "$1" == build ]]; then
	# Regenerate the doxygen documentation
	doxygen &> /dev/null

	# Build the makefiles using g++
	#+Clang seem to not like some of the design choices of Catch2
	compiler=$(command -v g++)
	cmake -H. -Bbuild -DCMAKE_CXX_COMPILER="$compiler" -DCMAKE_BUILD_TYPE="Release"
elif [[ "$1" == test ]]; then
	run_project Testing
else
	# On clean make, just run the project
	run_project IsingWorm
fi
