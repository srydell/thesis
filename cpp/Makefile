# -*- MakeFile -*-
#
# Example MakeFile 
# Copyright 2018 Simon Rydell, all rights reserved 
#
#  --- Syntax ---
#  target: dependencies
#  	action
#
all: main.out
	./main.out

main.out: main.o
	@g++ main.o -o main.out

main.o: main.cpp
	@g++ -std=c++17 -c main.cpp

clean:
	rm *.o main.out
