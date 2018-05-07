# -*- MakeFile -*-
#
# Example MakeFile 
# Copyright 2017 Simon Rydell, all rights reserved 
#
#  --- Syntax ---
#  target: dependencies
#  	action
#
all: program1 program2

program1: program1.o funcs.o
	g++ program1.o funcs.o -o program1

program1.o: program1.cpp funcs.h
	g++ -c program1.cpp

funcs.o: funcs.cpp
	g++ -c funcs.cpp

program2: program2.o funcs.o
	g++ program2.o funcs.o -o program2

program2.o: program2.cpp funcs.h
	g++ -c program2.cpp

clean:
	rm *.o program1 program2
