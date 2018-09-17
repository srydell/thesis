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
		if [[ -f ./"$1" ]]; then
			time ./"$1"
		fi
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
	declare -a tests=("UtilsTest" "IsingTest" "XYTest")
	for t in "${tests[@]}"
	do
		run_project "$t"
	done
elif [[ "$1" == i ]]; then
	# On clean make, just run the project
	run_project IsingWorm
elif [[ "$1" == x ]]; then
	# On clean make, just run the project
	run_project xyWorm
else
	run_project "$1"
fi
