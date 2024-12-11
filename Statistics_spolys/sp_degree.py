import os

def get_polynomial_degree(file_path):
    max_degree = 0
    with open(file_path, 'r', encoding='ISO-8859-1') as file:
        for line in file:
            variables = line.strip().split()
            degree = len(variables)
            if degree > max_degree:
                max_degree = degree               
    return max_degree




# Step 1: Define the folder path
round = 832
folder_path = "../Data_spolys/sp_"+str(round)

# Step 2: Collect the number of lines in each file
degree_counts = []

# Traverse all files in the directory
for filename in os.listdir(folder_path):
    file_path = os.path.join(folder_path, filename)
    if os.path.isfile(file_path):
        polynomial_degree = get_polynomial_degree(file_path)
        degree_counts.append(polynomial_degree)
        
freq_dict = {}
for x in degree_counts:
	freq_dict[x] = freq_dict.get(x,0) +1
res = sorted(freq_dict.items())
for ele in res:
    	print("degree_"+str(ele[0])+": "+str(ele[1]))