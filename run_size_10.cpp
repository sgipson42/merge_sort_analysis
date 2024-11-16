#!/bin/bash

# Check if the size argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <size>"
    exit 1
fi

# Read the size argument
SIZE=$10

# Compile the C++ program
g++ -o mergesort mergesort.cpp

# Run the compiled program with the size argument
./mergesort $SIZE

