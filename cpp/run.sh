#!/usr/bin/env bash

cd ./build || exit

function cmakeRunProject {
	# Run if build succeeds
	if cmake --build .; then
		time ./IsingWorm ;
	fi
}

if [[ "$1" == build ]]; then
	# Build the makefiles
	cmake ../

	# Regenerate the doxygen documentation
	# In build/doxygen
	cd doxygen || exit
	doxygen ../doxygen.conf &> /dev/null

	# Go back to build
	cd ../ || exit
	
	# Run the code
	cmakeRunProject
	exit
fi

# If not build, then run it via cmake
cmakeRunProject
