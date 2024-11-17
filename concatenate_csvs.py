import pandas as pd
import os

input_folder = "./" 
output_file = "merged_columns.csv"
all_data = []
for filename in os.listdir(input_folder):
	print(filename)
	if filename.endswith(".csv"):
		filepath = os.path.join(input_folder, filename)
		print(filepath)
		df = pd.read_csv(filepath)
		prefix = os.path.splitext(filename)[0]
		print(prefix)
		df = df.add_prefix(f"{prefix}_")
		all_data.append(df)

merged_data = pd.concat(all_data, axis=1)
merged_data.to_csv(output_file, index=False)
print(f"Merged CSV saved to {output_file}")
