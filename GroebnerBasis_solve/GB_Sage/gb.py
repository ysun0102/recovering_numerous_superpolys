from random import randint
import os
from sage.all import *
from datetime import datetime

def hex_to_bool(h):
    val = 0
    if h == "a":
        val = 10
    elif h == "b":
        val = 11
    elif h == "c":
        val = 12
    elif h == "d":
        val = 13
    elif h == "e":
        val = 14
    elif h == "f":
        val = 15
    else:
        val = int(h)

    res = [val % 2]
    val = val >> 1
    res += [val % 2]
    val = val >> 1
    res += [val % 2]    
    val = val >> 1
    res += [val % 2]
    assert((val >> 1) == 0)

    return res

###########################################
var_num=80
folder_path = "../Data_spolys/sp_"+str(round)
###########################################

print(datetime.now()) 
# R = PolynomialRing(GF(2), var_num, 'x',order='degrevlex')
R = BooleanPolynomialRing(var_num, 'x',order='degrevlex')
# R = PolynomialRing(GF(2), var_num, 'x',order='lex')
vars = R.gens() 
# print(R, vars)

## random key value
# random_values_list = [randint(0, 1) for _ in range(var_num)]
random_values_list = []
for ss in "8e713bbb2de659c97d15":
    random_values_list += hex_to_bool(ss)
random_values = {var: value for var,value in zip(vars,random_values_list)}
print("Random key value:", random_values)


def read_polynomial_from_file(filepath):
    poly = 0
    with open(filepath, 'r') as f:
        lines = f.readlines()
        if lines[-1].strip() == "":
            lines.pop()
        for line in lines:
            line = line.strip()
            if line == '': 
                poly += 1
            else:
                indices = list(map(int, line.split())) 
                term = 1
                for idx in indices:
                    term *= vars[idx]
                poly += term 
    return poly



txt_files = [os.path.join(folder_path, f) for f in os.listdir(folder_path)]
equations = []
for txt_file in txt_files:
    equations.append(read_polynomial_from_file(txt_file))

print("\nequation number: ", len(equations))
print(datetime.now())


eq_values=[eq.substitute(random_values) for eq in equations]
new_equations=[eq-val for eq,val in zip(equations,eq_values)]
# print("New equations: ", new_equations)


I = Ideal(new_equations)
G = I.groebner_basis()
# G = I.groebner_basis(faugere=True)
# G = I.groebner_basis(faugere=False)

print("\nGroebner basis:", G)


for g in G:
    print(g)


print(datetime.now())
