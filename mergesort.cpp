#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <bits/stdc++.h>

using namespace std

vector<int> merge(const vector<int>& left, const vector<int>& right) {
	// merge algorithm here
	return result;
}

vector<int> mergeSort1(const vector<int>& arr) {
	//mergesort1 algorithm here
	return merge(left, right);
}

vector<int> generateData(const int size) {
	vector<int> result(size); 
	// Set up a random number generator with a distribution for the desired range
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 100);
	
	generate(result.begin(), result.end(), [&]() { return dist(gen); });
	return result;
}

vector<int>& preSortData(const string& sortOption, const vector<int>& arr) {
	vector<int> copy = arr;
	if (sortOption == "reverse") {
		sort(copy.begin(), copy.end(), std::greater<int>())
	} else {
		sort(copy.begin(), copy.end());
	}

	if (sortOption == "nearly") {
		// move a couple around out of order
	}
	return copy;
}

long long runExperiment1(const vector<int>& arr) { 
	auto start = chrono::high_resolution_clock::now();
	mergeSort1(arr, 0, arr.size() - 1);
	auto end = chrono::high_resolution_clock::now();
}

vector<int>& loop_experiment(const vector<int>& v) {
	vector<int> times;
	for (i=0; i++; i<100) {
		long long time = runExperiment1(v);
		times.push_back(time)
	}
	return times;
}	

void printArr(const vector<int>& arr) {
	last_num = v[0]
	for (int num : v) {
		if (num < last_num) {
			cout << num << "is less than" << last_num << endl;
		} else {
			cout << num << " ";
		}
	}
    	cout << endl;
	return 0;


int main() {
	//TODO: make a shell script to pass in arr size as input, so can run multiple at a time
	//	maybe, but not the current focus
	//vector<int> sizes = {10, 100, 1000, 10000, 100000, 1000000};
	vector<int> sampleArr = {5, 4, 3, 6, 7, 8, 1, 2, 10, 9};
	cout << "Testing algorithms..." << endl;
	sortedArr1 = mergeSort1(sampleArr);
	printArr(sortedArr1);
	//sortedArr2 = mergeSort2(sampleArr);
	//printArr(sortedArr2);
	//sortedArr3 = mergeSort3(sampleArr);
	//printArr(sortedArr3);
	//sortedArr4 = mergeSort4(sampleArr);
	//printArr(sortedArr4);
	
	int size = stoi(argv[1]); 	
	cout << "Generating random data of size " << size << "..." << endl;
    	vector<int> v = generateData(size); 

	// RUN FIRST EXPERIMENT
	cout << "running experiment on unsorted data..." << endl;
	vector<int> unsorted_times = loop_experiment(v);

	cout << "running experiment on reversed data..." << endl;
	reversedArr = mergeSort1(("reverse", v));
	vector<int> reversed_times = loop_experiment(reversedArr);

	cout << "running experiment on nearly sorted data..." << endl;
	nearlySortedArr = mergeSort1(("nearly", v));
	vector<int> nearly_sorted_times = loop_experiment(nearlySortedArr);

	cout << "running experiment on sorted data..." << endl;
	sortedArr = mergeSort1(("sort", v));
	vector<int> sorted_times = loop_experiment(sortedArr);
	
	// MAKE A FUNCTION FOR GETTING AVERAGE
double calculateAve(const vector<int>& values) {
    double sum = 0;
    for (int value : values) {
        sum += value;
    }

    double average = (n > 0) ? (sum / n) : 0;  // Avoid division by zero

    // Output the average
    std::cout << "The average is: " << average << std::endl;
	// RUN NEXT EXPERIMENTS SAME SET UP
}

