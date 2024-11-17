#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <bits/stdc++.h>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

void merge1(const int h, const int m, const vector<int>& arr1, const vector<int>& arr2, vector<int>& arr) {
	int i = 0;
	int j = 0;
	int k = 0;
	while (i < h && j < m) {
		if (arr1[i] < arr2[j]) {
			arr[k] = arr1[i];
			i++;
		} else {
			arr[k] = arr2[j];
			j++;
		}
		k++;
	}
	if (i > h) {
		copy(arr2.begin()+j, arr2.begin()+m, arr.begin()+k);
	} else {
		copy(arr1.begin()+i, arr1.begin()+h, arr.begin()+k);
	}
}

void mergeSort1(const int n, vector<int>& arr) {
	if (n>1) {
		const int h = floor(n/2);
		const int m = n-h;
		vector<int> arr1(h);
		vector<int> arr2(m);
		
		copy(arr.begin(), arr.begin() + h, arr1.begin());
		copy(arr.begin()+h, arr.end(), arr2.begin());

		mergeSort1(h, arr1);
		mergeSort1(m, arr2);
		merge1(h,m, arr1, arr2, arr);
	}
}

void merge2(const int low, const int mid, const int high, vector<int>& arr) {
	vector<int> temp(high - low + 1);
	int i = low;
	int j = mid+1;
	int k = low;

	while (i<=mid && j<=high) {
		if (arr[i] < arr[j]) {
			temp[k-low] = arr[i];
			i++;
		} else {
			temp[k-low] = arr[j];
			j++;
		}
		k++;
	}
	if ( i > mid) {
		copy(arr.begin() + j, arr.begin() + high + 1, temp.begin() + k-low);
	} else {
		copy(arr.begin() + i, arr.begin() + mid + 1, temp.begin() + k-low);
	}
	copy(temp.begin(), temp.end(), arr.begin() + low);
}

void mergeSort2(const int low, const int high, vector<int>& arr) {
	int mid;
	if (low < high) {
		mid = floor((low + high)/2);
		mergeSort2(low, mid, arr);
		mergeSort2(mid+1, high, arr);
		merge2(low, mid, high, arr);
	}
}

vector<int> generateData(const int size) {
	vector<int> result(size); 
	// Set up a random number generator with a distribution for the desired range
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 1000000);
	
	generate(result.begin(), result.end(), [&]() { return dist(gen); });
	return result;
}

vector<int> preSortData(const string& sortOption, const vector<int>& arr) {
	vector<int> copy = arr;
	if (sortOption == "reverse") {
		sort(copy.begin(), copy.end(), std::greater<int>());
	} else {
		sort(copy.begin(), copy.end());
	}
	if (sortOption == "nearly") { //sorted already
				      // 1/5 of data move out of order
		cout << "swapping indices " << floor(copy.size()/5) << " times..." << endl;
		for (int i=0; i<floor(copy.size()/5); i++) {
			int idx1 = rand() % copy.size();
			int idx2 = rand() % copy.size();
			while (idx1 == idx2) {
				idx2 = rand() % copy.size();
			}
			swap(copy[idx1], copy[idx2]);
		}
	}
	return copy;
}

vector<long long> loop_experiment(vector<int>& v, string& algorithm) {
	vector<long long> times;
	if (algorithm == "1") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort1(v.size(), v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());
		}
	} else if (algorithm == "2") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort2(0, v.size()-1, v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());
		}
	}/*
	} else if (algorithm == "3") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort3(v.size(), v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());
		}
	} else if (algorithm == "4") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort4(v.size(), v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());
		}
	}
	*/
	return times;
}	

void printArr(const vector<int>& v) {
	for (int num : v) {
		cout << num << endl;
	}
    	cout << endl;
}

void write_data_to_csv(const string& filename, const vector<vector<long long>>& lists, const vector<string>& headers) {
	ofstream file(filename);
	
	for (size_t i = 0; i< headers.size(); i++) {
		file << headers[i];
		if (i!=headers.size() -1) {
			file << ",";
		}
	}
	file << "\n";
	for (size_t i = 0; i < 100; i++) { // loop through idxs of lists (rows)
		for (size_t j = 0; j<lists.size(); j++) { // loop through columns
			if (i < lists[j].size()) { // not on last row
				file << lists[j][i]; // writes to the jth column in the ith row
			}
			if (j < lists.size() - 1) {
				file << ",";
			}
		}
		file << "\n";
	}
	file.close();
}

int main(int argc, char* argv[]) {
	// TODO: pass in actual arr to mergesort1 and rerun (not by reference)
	// 	recheck all the types in mergesort1 and merge1
	//vector<int> sizes = {10, 100, 1000, 10000, 100000, 1000000};
	vector<string> headers = {"unsorted_times", "reversed_times", "nearly_sorted_times", "sorted_times"};

	int size = stoi(argv[1]); 	
	string filepath = argv[2]; 	
	string algorithm = argv[3];
	cout << "Generating random data of size " << size << "..." << endl;
    	vector<int> v = generateData(size); 
	printArr(v);

	// RUN FIRST EXPERIMENT
	cout << "running experiment on unsorted data..." << endl;
	vector<int> randomArr = preSortData("random", v);
	vector<long long> unsorted_times = loop_experiment(randomArr, algorithm);
	printArr(randomArr);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "reversing data..." << endl;
	vector<int> reversedArr = preSortData("reverse", v);
	printArr(reversedArr);
	cout << "running experiment on reversed data..." << endl;
	vector<long long> reversed_times = loop_experiment(reversedArr, algorithm);
	printArr(reversedArr);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "incompletely sorting data..." << endl;
	vector<int> nearlySortedArr = preSortData("nearly", v);
	printArr(nearlySortedArr);
	cout << "running experiment on nearly sorted data..." << endl;
	vector<long long> nearly_sorted_times = loop_experiment(nearlySortedArr, algorithm);
	printArr(nearlySortedArr);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "sorting data..." << endl;
	vector<int> sortedArr = preSortData("sort", v);
	printArr(sortedArr);
	cout << "running experiment on sorted data..." << endl;
	vector<long long> sorted_times = loop_experiment(sortedArr, algorithm);
	printArr(sortedArr);
	cout << "original:\n" << endl;
	printArr(v);

	
	vector<vector<long long>> all_times = {unsorted_times, reversed_times, nearly_sorted_times, sorted_times};


	write_data_to_csv(filepath + "_" + to_string(size) + ".csv", all_times, headers); 
	// RUN NEXT EXPERIMENTS SAME SET UP
}
