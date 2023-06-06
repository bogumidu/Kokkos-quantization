#!/bin/bash

EXECUTABLE="./cmake-build-debug/project2"

for((j=1; j<=24; j++))
do
	for((k=1; k<=45; k++))
	do
		for((i=1; i<=10; i++))
		do
			$EXECUTABLE ./model_files/skull.obj -t:$j -s:$k --log
		done
	done
done

