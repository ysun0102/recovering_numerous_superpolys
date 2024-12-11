import os
from pysat.formula import CNF
from pysat.solvers import Solver
import time
import random
import copy
from datetime import datetime

#####################################

print("\n[832-Trivium] The frequency statistics of secret variables k0~k79 appearing in 1373 superpolys\n")

#####################################


def read_polynomials(folder): 
    equations = {}
    used_vars = {}

    for filename in os.listdir(folder):
        file = open(os.path.join(folder, filename), "r")

        terms = []
        first_line_empty = False
        vars = set()
        for line in file:
            line = line.strip()
            if not terms and line == "":
                first_line_empty = True
            elif line:
                vars_in_line = [int(x) for x in line.split()]
                terms.append(vars_in_line)
                vars.update(vars_in_line)
        equations[filename] = (terms, first_line_empty)

        file.close()

        for ele in vars:
            if ele in used_vars:
                used_vars[ele] += 1
            else:
                used_vars[ele] = 1

    for i in range(80):
        if i not in used_vars:
            used_vars[i] = 0

    res = sorted(used_vars.items(),key=lambda item:item[1])
    for ele in res:
    	print("x"+str(ele[0])+": "+str(ele[1]))


    return equations, [it[0] for it in res]
   



if __name__ == "__main__":


    equations, used_vars = read_polynomials("../Data_spolys/sp_832")


