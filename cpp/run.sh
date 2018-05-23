#!/usr/bin/env bash

cd ./build || exit

if [[ "$1" == build ]]; then
	# Build the makefiles
	cmake ../
	
	# Run if build succeeds
	if cmake --build .; then
		time ./IsingWorm ;
	fi
	exit
fi

# Run if build succeeds
if cmake --build .; then
	time ./IsingWorm;
fi
