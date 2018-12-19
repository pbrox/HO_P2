#!/bin/bash
rm -rf ../testmaps/*.output
rm -rf ../testmaps/*.statistics
for f in ../testmaps/*.txt; do
        for i in sum_diagonal_distance; do
                echo "File $f, heuristic $i"
                build/AstarPaganitzu $f $i
                mv $f.output $f.$i.output
                mv $f.statistics $f.$i.statistics
        done
done

