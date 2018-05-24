#!/usr/bin/env bash

cd ./build || exit

if [[ "$1" == build ]]; then
	# Build the makefiles
	cmake ../ > /dev/null 2>&1
	
	# Run if build succeeds
	if cmake --build . > /dev/null 2>&1; then
		time ./IsingWorm ;
	fi
	exit
fi

# Run if build succeeds
if cmake --build . > /dev/null 2>&1; then
	time ./IsingWorm;
fi
