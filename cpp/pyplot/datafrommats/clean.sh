#!/usr/bin/env bash
## Maintainer: Simon Rydell
## Date created: Oct 03, 2018

files="$(ls w*)"

for f in $files
do
	echo "L=${f:1}:" >> test.out
	cat "$f" >> test.out
done
