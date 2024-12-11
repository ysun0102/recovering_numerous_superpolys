import os

target_variable = 2
variable_count_per_file = {}
folder_path = '../Data_spolys/sp_832'  

for filename in os.listdir(folder_path):

    file_path = os.path.join(folder_path, filename)
        
    variable_count = 0
       
    with open(file_path, 'r') as f:
        lines = f.readlines()
        for line in lines:
            line = line.strip()
            if line:
                indices = list(map(int, line.split()))
                if target_variable in indices:
                    variable_count += 1
        
    variable_count_per_file[filename] = variable_count,len(lines)

for filename, count in variable_count_per_file.items():
    print(f"k{target_variable} appear in {count[0]} monomials, all monomials: {count[1]} ")