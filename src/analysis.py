import pandas as pd
from sklearn.metrics import r2_score
import re
import matplotlib.pyplot as plt
from scipy.stats import linregress
import numpy as np
from rich.console import Console

console = Console()
data = pd.read_csv('../merged_columns.csv')
console.print(data)

averages = data.mean()
console.print(averages)

# Extract relevant execution time columns and array sizes
execution_time_cols = [col for col in data.columns if '_times_' in col]
memory_usage_cols = [col for col in data.columns if '_memory_' in col]

algs = ['mergesort1', 'mergesort2', 'mergesort3', 'mergesort4']
scenarios = ['unsorted', 'reversed', 'sorted', 'nearly_sorted']

# separate memories and times into mergsort algs
mergesort_time_cols = []
mergesort_memory_cols = []
for alg in algs:
	time_cols = [col for col in execution_time_cols if alg in col]
	for scenario in scenarios:
		if scenario == 'sorted':
			scen_cols = [col for col in time_cols if 'times_'+scenario in col]
			mergesort_time_cols.append(scen_cols)
		else: 
			scen_cols = [col for col in time_cols if scenario in col]
			mergesort_time_cols.append(scen_cols)

	mem_cols = [col for col in memory_usage_cols if alg in col]
	for scenario in scenarios:
		if scenario == 'sorted':
			scen_cols = [col for col in mem_cols if 'memory_'+scenario in col]
			mergesort_memory_cols.append(scen_cols)
		else: 
			scen_cols = [col for col in mem_cols if scenario in col]
			mergesort_memory_cols.append(scen_cols)
console.print(mergesort_time_cols)
console.print(mergesort_memory_cols)
"""
# MAKE INDIVIDUAL MEMORY GRAPHS
for section in mergesort_memory_cols:	
	algorithm = next((alg for alg in algs if alg in section[0]), None)
	scenario = next((sc for sc in scenarios if sc in section[0]), None)

	vals = []
	sizes = []
	for col in section:
		val = averages[col]
		vals.append(val)
		
		parts = col.split('_')
		number = int(parts[1])
		sizes.append(number)
		
	sizes, vals = zip(*sorted(zip(sizes, vals)))
	plt.figure(figsize=(20, 5))
	plt.scatter(sizes, vals, label="Data Points")
	z = np.polyfit(sizes, vals, 1)
	p = np.poly1d(z)
	plt.plot(sizes, p(sizes), linestyle='--', label=f"Trend Line (y = {z[0]:.2e}x + {z[1]:.2e})")

	plt.title(f"Algorithm: {algorithm}, Scenario: {scenario}")
	plt.xlabel("Array Size")
	plt.ylabel("Values")
	plt.legend()
	plt.grid()

	filename = f"../plots/memory_{algorithm}_{scenario}_plot.png"
	plt.savefig(filename)
	print(f"Saved figure as {filename}")
	plt.show()
# MAKE INDIVIDUAL TIME GRAPHS
for section in mergesort_time_cols:	
	algorithm = next((alg for alg in algs if alg in section[0]), None)
	scenario = next((sc for sc in scenarios if sc in section[0]), None)

	vals = []
	sizes = []
	for col in section:
		val = averages[col]
		vals.append(val)
		
		parts = col.split('_')
		number = int(parts[1])
		sizes.append(number)
		
	sizes, vals = zip(*sorted(zip(sizes, vals)))
	plt.figure(figsize=(20, 5))
	plt.scatter(sizes, vals, label="Data Points")
	z = np.polyfit(sizes, vals, 1)
	p = np.poly1d(z)
	plt.plot(sizes, p(sizes), linestyle='--', label=f"Trend Line (y = {z[0]:.2e}x + {z[1]:.2e})")

	plt.title(f"Algorithm: {algorithm}, Scenario: {scenario}")
	plt.xlabel("Array Size")
	plt.ylabel("Values")
	plt.legend()
	plt.grid()

	filename = f"../plots/times_{algorithm}_{scenario}_plot.png"
	plt.savefig(filename)
	print(f"Saved figure as {filename}")
	plt.show()
"""
# COMBINE PER DATA SCENARIO 
mergesort_time_cols = []
mergesort_memory_cols = []
for alg in algs:
	time_cols = [col for col in execution_time_cols if alg in col]
	mergesort_time_cols.append(time_cols)

	mem_cols = [col for col in memory_usage_cols if alg in col]
	mergesort_memory_cols.append(mem_cols)

console.print(mergesort_time_cols)
console.print(mergesort_memory_cols)

#console.print(list(averages.index))
console.print(averages)
memory_scenarios_cols = []
for scenario in scenarios:
	filtered_list = [item for item in averages.index if f'memory_{scenario}_usage' in item]
	memory_scenarios_cols.append(filtered_list)		
console.print(memory_scenarios_cols)

times_scenarios_cols = []
for scenario in scenarios:
	filtered_list = [item for item in averages.index if f'times_{scenario}_usage' in item]
	times_scenarios_cols.append(filtered_list)		

# Generate plots for each scenario
#MEMORY PERFORMANCE SHOULD BE LINEAR
for scenario in scenarios:
    plt.figure(figsize=(10, 6))
    for alg in algs:
        # Filter relevant data for the current algorithm and scenario
        filtered_data = [item for item in averages.index if f'{alg}_' in item and f'memory_{scenario}_usage' in item]
        sizes = []
        values = []
        
        for col in filtered_data:
            # Extract size from the string
            parts = col.split('_')
            size = int(parts[1])
            sizes.append(size)
            values.append(averages[col])
        
        # Sort by size
        sizes, values = zip(*sorted(zip(sizes, values)))
        #plt.plot(sizes, values, marker='o', label=f"{alg}")
        plt.scatter(sizes, values, label=f"{alg}")
        z = np.polyfit(sizes, values, 1)
        p = np.poly1d(z)
        predicted_vals = p(sizes)
        r_squared = r2_score(values, predicted_vals)
        plt.plot(sizes, predicted_vals, linestyle='--', label=f"Trend Line: y = {z[0]:.2e}x + {z[1]:.2e}\nR² = {r_squared:.4f}")
        #plt.plot(sizes, p(sizes), linestyle='--', label=f"{alg}\nTrend Line (y = {z[0]:.2e}x + {z[1]:.2e})")

    plt.title(f"Mergesort Memory Performance on {scenario.capitalize()} Data")
    plt.xlabel("Array Size")
    plt.ylabel("Bytes")
    plt.legend()
    plt.grid()
    plt.savefig(f"Mergesort_Memory_Performance_on_{scenario.capitalize()}_Data.png")

    plt.show()
# TIME PERFORMANCE SHOULD BE NLOGN
for scenario in scenarios:
    plt.figure(figsize=(10, 6))
    for alg in algs:
        # Filter relevant data for the current algorithm and scenario
        filtered_data = [item for item in averages.index if f'{alg}_' in item and f'times_{scenario}' in item]
        sizes = []
        values = []
        
        for col in filtered_data:
            # Extract size from the string
            parts = col.split('_')
            size = int(parts[1])
            sizes.append(size)
            values.append(averages[col])
        
        # Sort by size
        sizes, values = zip(*sorted(zip(sizes, values)))
        #plt.plot(sizes, values, marker='o', label=f"{alg}")
        plt.scatter(sizes, values, label=f"{alg}")
        sizes_log = [s * np.log(s) for s in sizes]
        z = np.polyfit(sizes_log, values, 1)
        p = np.poly1d(z)

        predicted_vals = p(sizes_log)
        r_squared = r2_score(values, predicted_vals)
        plt.plot(sizes, predicted_vals, linestyle='--', label=f"Trend Line: y = {z[0]:.2e} * nlogn + {z[1]:.2e}\nR² = {r_squared:.4f}")
        #plt.plot(sizes, p(sizes), linestyle='--', label=f"{alg}\nTrend Line (y = {z[0]:.2e}x + {z[1]:.2e})")

    # Add title, labels, and legend
    plt.title(f"Mergesort Time Performance on {scenario.capitalize()} Data")
    plt.xlabel("Array Size")
    plt.ylabel("Nanoseconds")
    plt.legend()
    plt.grid()
    plt.savefig(f"Mergesort_Time_Performance_on_{scenario.capitalize()}_Data.png")

    # Show the graph
    plt.show()
	
	
"""
for section in mergesort_memory_cols:	
	algorithm = next((alg for alg in algs if alg in section[0]), None)
	scenario = next((sc for sc in scenarios if sc in section[0]), None)

	vals = []
	sizes = []
	for col in section:
		val = averages[col]
		vals.append(val)
		
		parts = col.split('_')
		number = int(parts[1])
		sizes.append(number)
		
	sizes, vals = zip(*sorted(zip(sizes, vals)))
	plt.figure(figsize=(20, 5))
	plt.scatter(sizes, vals, label="Data Points")
	z = np.polyfit(sizes, vals, 1)
	p = np.poly1d(z)
	plt.plot(sizes, p(sizes), linestyle='--', label=f"Trend Line (y = {z[0]:.2e}x + {z[1]:.2e})")

	plt.title(f"Algorithm: {algorithm}, Scenario: {scenario}")
	plt.xlabel("Array Size")
	plt.ylabel("Values")
	plt.legend()
	plt.grid()

	filename = f"memory_{algorithm}_{scenario}_plot.png"
	plt.savefig(filename)
	print(f"Saved figure as {filename}")
	plt.show()
"""
