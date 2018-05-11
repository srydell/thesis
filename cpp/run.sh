#!/usr/bin/env bash

if [[ "$1" == build ]]; then
	cd ./build || exit
	# Build the makefiles
	cmake ../
	# Run if make succeeds
	if make; then
		./IsingWorm ;
	fi
	exit
fi

cd ./build || exit
# Run if make succeeds
if make; then
	./IsingWorm;
fi
