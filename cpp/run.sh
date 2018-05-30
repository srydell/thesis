#!/usr/bin/env bash

cd ./build || exit

function cmakeRunProject {
	# Run if build succeeds
	# if cmake --build .; then
	if make --no-print-directory; then
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
	cmakeRunProject
	exit
fi

# If not build, then run it via cmake
cmakeRunProject
