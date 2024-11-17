#!/bin/bash

sizes=(10 100 1000 10000 100000 1000000)
algorithms=("mergesort1 1" "mergesort2 2")

for algo in "${algorithms[@]}"; do
    for size in "${sizes[@]}"; do
        ./mergeSort "$size" $algo
    done
done

concatenate_csvs.py
