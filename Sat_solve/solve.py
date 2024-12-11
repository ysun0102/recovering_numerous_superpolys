import os
from pysat.formula import CNF
from pysat.solvers import Solver
import time
import random
import copy
from datetime import datetime

#####################################

print("832-Trivium-version 5.0")
print("start")

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
                vars_in_line = [int(x) + 1 for x in line.split()]
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
        if (i + 1) not in used_vars:
            used_vars[i + 1] = 0

    res = sorted(used_vars.items(),key=lambda item:item[1])

    return equations, [it[0] for it in res]
    


def convert_to_cnf(equations, values):
    cnf = CNF()
    var_count = 80
    term_to_var = {}
    xor_clause_s = []

    for filename, (terms, first_line_empty) in equations.items():
        xor_clause = []
        if first_line_empty:
            rhs = 1
        else:
            rhs = 0

        for term in terms:
            if len(term) == 1:
                xor_clause.append(term[0])
            else:  
                term_tuple = tuple(sorted(term))

                if term_tuple not in term_to_var:
                    intermediate_var = var_count + 1
                    var_count += 1
                    term_to_var[term_tuple] = intermediate_var

                    for var in term:
                        # !y || x1
                        # !y || x2
                        # !y || x3
                        cnf.append([-intermediate_var, var])
                    #  y || !x1 || !x2 || !x3
                    cnf.append([intermediate_var] + [-var for var in term])
                else:
                    intermediate_var = term_to_var[term_tuple]
                xor_clause.append(intermediate_var)

        if values[filename] == 1:
            rhs = 1 - rhs

        xor_clause_s.append((xor_clause, rhs))

    return cnf, xor_clause_s


def solve_cnf(cnf, xor_clause_s, assigns):
    solver = Solver(name="cryptominisat")
    solver.append_formula(cnf.clauses)
    for xor_clause, rhs in xor_clause_s:
        solver.add_xor_clause(xor_clause, rhs)

    print("cnf: ", ["k%d=1" % (row[0]-1) if row[0]>0 else "k%d=0" % (-(row[0]+1)) for row in assigns])
    for assumption in assigns:
        solver.add_clause(assumption)

    if solver.solve():
        solution = solver.get_model()
        print("\n\tFind solution~~~\n")
        return True
    else:
        print("\n\tNo solution!\n")
        return False





def auto_solve(cnf, xor_clause_s, used_vars, inc_var, current_assigns=[]):
    
    if inc_var == []:
    	inc_var=copy.copy(used_vars)
    print([x-1 for x in inc_var])
    print([[x-1 if x>0 else x+1 for x in row] for row in current_assigns])
    both_has_solutions = []
    last_both_has_solutions = []


    while len(inc_var) > 0:

        to_test_var = inc_var[-1]
        inc_var = inc_var[:-1]

        print(datetime.now())
        print("round %d" % (len(current_assigns) + len(both_has_solutions)))
        print("untested vars: ", [x-1 for x in inc_var])
        print("to test var: %d" % (to_test_var-1))        
        print("current_assigns: ", ["k%d=1" % (row[0]-1) if row[0]>0 else "k%d=0" % (-(row[0]+1)) for row in current_assigns], len(current_assigns))
        print("both_has_solutions: ", [x-1 for x in both_has_solutions], len(both_has_solutions))

        print("testing k%d == 0" % (to_test_var-1))
        flag_0 = solve_cnf(cnf, xor_clause_s, current_assigns + [[-to_test_var]])

        if not flag_0:
            current_assigns += [[to_test_var]] 
            print("i jumped")
            print("round is done\n")
        else:
            print("testing k%d == 1" % (to_test_var-1))
            flag_1 = solve_cnf(cnf, xor_clause_s, current_assigns + [[to_test_var]])

            print("round is done\n")

            if flag_0 and flag_1:
                both_has_solutions += [to_test_var]
            else:
                if not flag_0 and not flag_1:
                    print("No solution at all, which is impossible!!!")
                    exit(1)

                if flag_0:
                    current_assigns += [[-to_test_var]]
                else:
                    current_assigns += [[to_test_var]]

    return both_has_solutions, current_assigns


def evaluate_polynomial(polynomial, hasone_flag, sol_val):
    result = 0
    if hasone_flag:
        result += 1
    for i, term in enumerate(polynomial):
        flag = True
        for item in term:
            if item > 79:
                assert(0)
            if(sol_val[item] == 0):
                flag = False
                break
        if flag == True:
            result += 1
    return result


def read_polynomials_from_file(file_path): 

    f = open(file_path, 'r')
    all_lines = f.readlines()
    f.close()
    assert(all_lines[0] != "\n" or all_lines[1] != "\n")

    polynomials = []
    hasone_flag = False
    with open(file_path, 'r') as file:
        first_line = file.readline().strip()
        if not first_line:
            hasone_flag = True
        else:
            monomial = list(map(int, first_line.split()))
            polynomials.append(monomial)
        for line in file:
            monomial = list(map(int, line.strip().split()))
            polynomials.append(monomial)

    return polynomials, hasone_flag


def bool_to_hex(b):
    assert(len(b) == 4)
    ss = b[0] + b[1] * 2 + b[2] * 4 + b[3] * 8

    if ss < 10:
        return str(ss)
    elif ss == 10:
        return "a"
    elif ss == 11:
        return "b"
    elif ss == 12:
        return "c"
    elif ss == 13:
        return "d"
    elif ss == 14:
        return "e"
    else:
        return "f"

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

def main_solve(directory, sol_val, both_has_solutions=[], current_assigns=[]):
    equations, used_vars = read_polynomials(directory)

    values = {}
    for filename in os.listdir(directory):                
        file_path = os.path.join(directory, filename)        
        # print(file_path)       
        polynomials, hasone_flag = read_polynomials_from_file(file_path)
        result = evaluate_polynomial(polynomials, hasone_flag, sol_val)
        result_mod = result % 2
        # print(filename," :", result_mod)
        values[filename.split("/")[-1]] = result_mod

    assert(len(equations) == len(values))

    cnf, xor_clause_s = convert_to_cnf(equations, values)
    both_has_solutions, current_assigns = auto_solve(cnf, xor_clause_s, used_vars, both_has_solutions, current_assigns)
    return both_has_solutions, current_assigns




if __name__ == "__main__":

    while True:

        start_time = time.perf_counter()
        
        dir_init = "polynomials/polynomial-"
        break_num = 0
        # sol_val = [random.randint(0, 1) for _ in range(80)]

        # preset value
        sol_val = []
        for ss in "8e713bbb2de659c97d15":
            sol_val += hex_to_bool(ss)

        print(sol_val)
        assert(len(sol_val) == 80)    

        output_name = ""
        for ii in range(int(len(sol_val)/4)):
            output_name += bool_to_hex(sol_val[ii*4: (ii+1)*4])
        print(output_name)
        
        both_has_solutions=[]
        current_assigns=[]
        
        round_num = 0
        for i in range(5):
            round_num = i
            directory = dir_init + f"{i}"

            print("now caculate dic:" , directory)

            both_has_solutions, current_assigns = main_solve(directory, sol_val, both_has_solutions, current_assigns)
            if len(both_has_solutions) <= break_num :
                break
        
        os.system("mkdir log_resolve")
        with open(("log_resolve/%02d" %  len(both_has_solutions)) + "_" + output_name + ".log", "w") as f:
            f.write(str(sol_val) + "\n")
            f.write(str([x-1 for x in both_has_solutions]) + "\n")
            f.write(str([[x-1 if x>0 else x+1 for x in row] for row in current_assigns]) + "\n")
            f.write(str(round_num) + "\n")
            print(output_name + "_" + str(len(both_has_solutions)) + ".log")


        end_time = time.perf_counter()
        print(f"Time cost: {end_time - start_time:.2f}s.")


