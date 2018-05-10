#!/usr/bin/env bash

if [[ "$1" == build ]]; then
	cd ./build || exit
	cmake ../
	make
	./IsingWorm
	exit
fi

cd ./build || exit
make
./IsingWorm
