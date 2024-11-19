import pandas as pd
import re
import matplotlib.pyplot as plt
from scipy.stats import linregress
import numpy as np
from rich.console import Console

console = Console()
data = pd.read_csv('merged_columns.csv')
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

console.print(list(averages.index))
console.print(averages)
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
	plt.figure(figsize=(8, 5))
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
def plot_group_with_trendline(groups, series):
    for group in groups:
        array_sizes = []
        values = []
        for key in group:
            match = re.search(r'_(\d+)_memory', key)
            if match:
                array_size = int(match.group(1))
                array_sizes.append(array_size)
                values.append(series[key])
        # Sort data by array size
        array_sizes, values = zip(*sorted(zip(array_sizes, values)))
        
        # Plot data
        plt.figure(figsize=(8, 5))
        plt.scatter(array_sizes, values, label="Data Points")
        
        # Add trend line
        z = np.polyfit(array_sizes, values, 1)
        p = np.poly1d(z)
        plt.plot(array_sizes, p(array_sizes), linestyle='--', label="Trend Line")
        
        # Labels and title
        plt.xlabel("Array Size")
        plt.ylabel("Value")
        plt.title(f"Group: {group[0].split('_memory')[0]}")
        plt.legend()
        plt.grid()
        plt.show()

# Plot all groups
plot_group_with_trendline(mergesort_memory_cols, averages)
plot_group_with_trendline(mergesort_time_cols, averages)
		 
"""
	
"""
# separate mergesort algs into scenarios
# Calculate averages for each column
execution_time_averages = data[execution_time_cols].mean()
memory_usage_averages = data[memory_usage_cols].mean()

# Extract array sizes from column names (assuming sizes are consistent in naming)
execution_time_sizes = [int(col.split('_')[1]) for col in execution_time_cols]
memory_usage_sizes = [int(col.split('_')[1]) for col in memory_usage_cols]

# Prepare data for plotting execution times
execution_time_df = pd.DataFrame({
    'Array Size': execution_time_sizes,
    'Average Execution Time': execution_time_averages.values
}).sort_values('Array Size')

# Plot execution time vs. array size
plt.figure(figsize=(10, 6))
for col in execution_time_cols:
    array_size = int(col.split('_')[1])
    avg_time = data[col].mean()
    plt.plot(array_size, avg_time, 'o', label=col.split('_')[0], alpha=0.6)

# Trend line for execution time
x = execution_time_df['Array Size']
y = execution_time_df['Average Execution Time']
z = np.polyfit(x, y, 2)
p = np.poly1d(z)
plt.plot(x, p(x), '--', label='Trend Line', color='red')

# Customize the plot
plt.title('Execution Time vs Array Size')
plt.xlabel('Array Size')
plt.ylabel('Average Execution Time (ms)')
plt.legend(loc='best')
plt.grid(True)
plt.show()

# Repeat the process for memory usage
memory_usage_df = pd.DataFrame({
    'Array Size': memory_usage_sizes,
    'Average Memory Usage': memory_usage_averages.values
}).sort_values('Array Size')

plt.figure(figsize=(10, 6))
for col in memory_usage_cols:
    array_size = int(col.split('_')[1])
    avg_memory = data[col].mean()
    plt.plot(array_size, avg_memory, 'o', label=col.split('_')[0], alpha=0.6)

# Trend line for memory usage
x = memory_usage_df['Array Size']
y = memory_usage_df['Average Memory Usage']
z = np.polyfit(x, y, 2)
p = np.poly1d(z)
plt.plot(x, p(x), '--', label='Trend Line', color='red')

# Customize the plot
plt.title('Memory Usage vs Array Size')
plt.xlabel('Array Size')
plt.ylabel('Average Memory Usage (MB)')
plt.legend(loc='best')
plt.grid(True)
plt.savefig('memory_usage_v_array_size.png')
"""
