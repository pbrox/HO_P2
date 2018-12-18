#!/bin/bash

for f in ../testmaps/*.txt; do
	for i in min_manhattan_distance sum_manhattan_distance; do
		echo "File $f, heuristic $i"
		build/AstarPaganitzu $f $i
	done
done
