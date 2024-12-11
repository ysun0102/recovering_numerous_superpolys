import os
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm
import matplotlib.colors as mcolors

# Step 1: Define the folder path
round = 832
folder_path = "../Data_spolys/sp_"+str(round)

# Step 2: Collect the number of lines in each file
line_counts = []

# Traverse all files in the directory
for filename in os.listdir(folder_path):
    file_path = os.path.join(folder_path, filename)
    if os.path.isfile(file_path):
        try:
            with open(file_path, 'r', encoding='utf-8') as file:
                # Count the number of lines in the file
                num_lines = sum(1 for line in file)
                line_counts.append(num_lines)
        except Exception as e:
            print(f"Error reading file {file_path}: {e}")

# Define bin width and edges
# bin_width = 2000
bin_width = 5000
bins = range(0, max(line_counts) + bin_width, bin_width)
hist_data, bin_edges = np.histogram(line_counts, bins=bins)

print(hist_data)
print(bin_edges)