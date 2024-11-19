#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <bits/stdc++.h>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstdlib>

using namespace std;

size_t currentMemory = 0;
size_t peakMemory = 0;

struct node {
	int key;
	int link;
};

class ScopedMemoryTracker {
public:
    ScopedMemoryTracker() {
        currentMemory = 0;
        peakMemory = 0;
    }

    size_t getPeakMemory() const {
        return peakMemory;
    }

    ~ScopedMemoryTracker() {
        // Optionally log peak memory here or do nothing
    }
};

void* operator new(size_t size) {
    currentMemory += size;
    if (currentMemory > peakMemory) {
        peakMemory = currentMemory;
    }
    return malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept {
    currentMemory -= size;
    free(ptr);
}

vector<node> convertToNodes(const vector<int>& data) {
    vector<node> nodes(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        nodes[i].key = data[i];
        if (i == data.size() - 1) {
            nodes[i].link = 0; // End of list
        } else {
            nodes[i].link = i + 1; // Link to the next node
        }
    }

    return nodes;
}

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

void merge3(const int low, const int mid, const int high, vector<int>& arr) {
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

void mergeSort3(const int n, vector<int>& arr) {
	int m, low, mid, high, size;
	m = pow(2, log2(n)); // this is just n
	// m = n;
	size = 1;
	while (size < m) {
		//for (low = 0; low <= m-2*size+1; low = low+2*size) {
		for (low = 0; low + size -1 < n; low += 2*size) {
			mid = low+size-1;
			high = min(low+2*size-1, n-1);
			merge3(low, mid, high, arr);
		}
		size = 2 * size;
	}
}

void merge4(int list1, int list2, int& mergedlist, vector<node>& arr) {
	int lastsorted;
	if (arr[list1].key < arr[list2].key) {
		mergedlist = list1;
		list1 = arr[list1].link;
	} else {
		mergedlist = list2;
		list2 = arr[list2].link;
	}
	lastsorted = mergedlist;
	while (list1 != 0 && list2 != 0) {
		if (arr[list1].key < arr[list2].key) {
			arr[lastsorted].link = list1;
			lastsorted = list1;
			list1 = arr[list1].link;
		} else {
			arr[lastsorted].link = list2;
			lastsorted = list2;
			list2 = arr[list2].link;
		}
	}
	if (list1 == 0){
		arr[lastsorted].link = list2;
	} else {
		arr[lastsorted].link = list1;
	}
}

void mergeSort4(const int low, const int high, int& mergedlist, vector<node>& arr) {
	int mid, list1, list2;
	if (low == high) {
		mergedlist = low;
		arr[mergedlist].link = 0;
	} else {
		mid = floor((low + high) / 2);
		mergeSort4(low, mid, list1, arr);
		mergeSort4(mid+1, high, list2, arr);
		merge4(list1, list2, mergedlist, arr);
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

void printArr(const vector<int>& v) {
	for (int num : v) {
		cout << num << endl;
	}
    	cout << endl;
}

void printLinkedList(int head, const vector<node>& nodes) {
    int current = head;
    while (current != 0) { // Traverse until the end of the list
        cout << nodes[current].key << " ";
        current = nodes[current].link; // Move to the next node
    }
    cout << endl;
}

vector<long long> time_experiment(vector<int>& v, string& algorithm) {
	vector<long long> times;
	vector<int> backup = v; // ensure that the unsorted data is being used each time
	if (algorithm == "1") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort1(v.size(), v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());

			if (i < 99) {
				v = backup;
			}
		}
	} else if (algorithm == "2") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort2(0, v.size()-1, v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());

			if (i < 99) {
				v = backup;
			}
		}
	} else if (algorithm == "3") {
		for (int i=0; i<100; i++) {
			auto start = chrono::high_resolution_clock::now();
			mergeSort3(v.size(), v);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());

			if (i < 99) {
				v = backup;
			}
		}
	} else if (algorithm == "4") {
		vector<node> nodes = convertToNodes(v);
		vector<node> backup = nodes;
		for (int i=0; i<100; i++) {
			int mergedlist;
			auto start = chrono::high_resolution_clock::now();
			mergeSort4(0, v.size(), mergedlist, nodes);
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
			times.push_back(duration.count());

			if (i < 99) {
				nodes = backup;
			}
		}
	} else {
		cout << "invalid algorithm choice." << endl;
	}
	if (algorithm == "4") {
    		int mergedlist;
		vector<node> nodes = convertToNodes(v);
    		mergeSort4(0, v.size(), mergedlist, nodes);
    		cout << "Sorted linked list:" << endl;
    		printLinkedList(mergedlist, nodes);
	} else {
    		printArr(v);
	}

	return times;
}	
vector<size_t> space_experiment(vector<int>& v, string& algorithm) {
	vector<size_t> results;
	vector<int> backup = v; // ensure that the unsorted data is being used each time
	if (algorithm == "1") {
		for (int i=0; i<100; i++) {
			ScopedMemoryTracker tracker;  // Start tracking memory
			mergeSort1(v.size(), v);
            		results.push_back(tracker.getPeakMemory());

			if (i < 99) {
				v = backup;
			}
		}
	} else if (algorithm == "2") {
		for (int i=0; i<100; i++) {
			ScopedMemoryTracker tracker;  // Start tracking memory
			mergeSort2(0, v.size()-1, v);
            		results.push_back(tracker.getPeakMemory());

			if (i < 99) {
				v = backup;
			}
		}
	} else if (algorithm == "3") {
		for (int i=0; i<100; i++) {
			ScopedMemoryTracker tracker;  // Start tracking memory
			mergeSort3(v.size(), v);
            		results.push_back(tracker.getPeakMemory());

			if (i < 99) {
				v = backup;
			}
		}
	} else if (algorithm == "4") {
		vector<node> nodes = convertToNodes(v);
		vector<node> backup = nodes;
		for (int i=0; i<100; i++) {
			int mergedlist;
			ScopedMemoryTracker tracker;  // Start tracking memory
			mergeSort4(0, v.size(), mergedlist, nodes);
            		results.push_back(tracker.getPeakMemory());

			if (i < 99) {
				nodes = backup;
			}
		}
	} else {
		cout << "invalid algorithm choice." << endl;
	}
	if (algorithm == "4") {
    		int mergedlist;
		vector<node> nodes = convertToNodes(v);
    		mergeSort4(0, v.size(), mergedlist, nodes);
    		cout << "Sorted linked list:" << endl;
    		printLinkedList(mergedlist, nodes);
	} else {
    		printArr(v);
	}

	return results;
}	

template <typename T>
void write_data_to_csv(const string& filename, const vector<vector<T>>& lists, const vector<string>& headers) {
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
	// 	recheck all the types in mergesort1 and merge1
	//vector<int> sizes = {10, 100, 1000, 10000, 100000, 1000000};
	vector<string> headers = {"unsorted_times", "reversed_times", "nearly_sorted_times", "sorted_times"};
	vector<string> memory_headers = {"unsorted_usage", "reversed_usage", "nearly_sorted_usage", "sorted_usage"};

	int size = stoi(argv[1]); 	
	string filepath = argv[2]; 	
	string algorithm = argv[3];
	cout << "Generating random data of size " << size << "..." << endl;
    	vector<int> v = generateData(size); 
	printArr(v);

	// RUN TIME EXPERIMENT
	cout << "running experiment on unsorted data..." << endl;
	vector<int> randomArr = preSortData("random", v);
	vector<long long> unsorted_times = time_experiment(randomArr, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "reversing data..." << endl;
	vector<int> reversedArr = preSortData("reverse", v);
	printArr(reversedArr);
	cout << "running experiment on reversed data..." << endl;
	vector<long long> reversed_times = time_experiment(reversedArr, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "incompletely sorting data..." << endl;
	vector<int> nearlySortedArr = preSortData("nearly", v);
	printArr(nearlySortedArr);
	cout << "running experiment on nearly sorted data..." << endl;
	vector<long long> nearly_sorted_times = time_experiment(nearlySortedArr, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "sorting data..." << endl;
	vector<int> sortedArr = preSortData("sort", v);
	printArr(sortedArr);
	cout << "running experiment on sorted data..." << endl;
	vector<long long> sorted_times = time_experiment(sortedArr, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	
	vector<vector<long long>> all_times = {unsorted_times, reversed_times, nearly_sorted_times, sorted_times};

	write_data_to_csv(filepath + "_" + to_string(size) + "_times.csv", all_times, headers); 

	// RUN SPACE EXPERIMENT
	cout << "running experiment on unsorted data..." << endl;
	vector<int> randomArr2 = preSortData("random", v);
	vector<size_t> unsorted_usages = space_experiment(randomArr2, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "reversing data..." << endl;
	vector<int> reversedArr2 = preSortData("reverse", v);
	printArr(reversedArr2);
	cout << "running experiment on reversed data..." << endl;
	vector<size_t> reversed_usages = space_experiment(reversedArr2, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "incompletely sorting data..." << endl;
	vector<int> nearlySortedArr2 = preSortData("nearly", v);
	printArr(nearlySortedArr2);
	cout << "running experiment on nearly sorted data..." << endl;
	vector<size_t> nearly_sorted_usages = space_experiment(nearlySortedArr2, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	cout << "sorting data..." << endl;
	vector<int> sortedArr2 = preSortData("sort", v);
	printArr(sortedArr2);
	cout << "running experiment on sorted data..." << endl;
	vector<size_t> sorted_usages = space_experiment(sortedArr2, algorithm);
	cout << "original:\n" << endl;
	printArr(v);

	vector<vector<size_t>> all_usages = {unsorted_usages, reversed_usages, nearly_sorted_usages, sorted_usages};

	write_data_to_csv(filepath + "_" + to_string(size) + "_memory.csv", all_usages, memory_headers); 
}
