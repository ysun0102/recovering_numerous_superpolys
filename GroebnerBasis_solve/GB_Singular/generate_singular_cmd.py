from random import randint
import os
from sage.all import *
from datetime import datetime
import time

# log recorder
class Logger(object):

    def __init__(self, name,stream=sys.stdout):
        output_dir = "./singular_cmds/"  # folder 
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        log_name_time = time.strftime('%Y-%m-%d-%H-%M-%S',time.localtime(time.time()))
        log_name = "gb_"+name+".sing"
        filename = os.path.join(output_dir, log_name)

        self.terminal = stream
        self.log = open(filename, 'a+')

    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)

    def flush(self):
        pass

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

###########################################
var_num=80
folder_path = "../Data_spolys/sp_"+str(round)
###########################################

# R = PolynomialRing(GF(2), var_num, 'x',order='degrevlex')
R = BooleanPolynomialRing(var_num, 'x',order='degrevlex')
# R = PolynomialRing(GF(2), var_num, 'x',order='lex')
vars = R.gens() 

## random key value
# random_values_list = [randint(0, 1) for _ in range(var_num)]
random_values_list = []
for ss in "95b82575c9814abd6e90":
    random_values_list += hex_to_bool(ss)
random_values = {var: value for var,value in zip(vars,random_values_list)}
output_name = ""
for ii in range(int(len(random_values_list)/4)):
    output_name += bool_to_hex(random_values_list[ii*4: (ii+1)*4])


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


eq_values=[eq.substitute(random_values) for eq in equations]
# print("eq values:", eq_values)

new_equations=[eq-val for eq,val in zip(equations,eq_values)]
# print("New equations: ", new_equations)

name = str(len(txt_files)) + "_" + output_name

sys.stdout = Logger(name,sys.stdout) 
sys.stderr = Logger(name,sys.stderr) 

print("option(prot);\n")

print("ring R = 2, (x_0,x_1,x_2,x_3,x_4,x_5,x_6,x_7,x_8,x_9,x_10,x_11,x_12,x_13,x_14,x_15,x_16,x_17,x_18,x_19,x_20,x_21,x_22,x_23,x_24,x_25,x_26,x_27,x_28,x_29,x_30,x_31,x_32,x_33,x_34,x_35,x_36,x_37,x_38,x_39,x_40,x_41,x_42,x_43,x_44,x_45,x_46,x_47,x_48,x_49,x_50,x_51,x_52,x_53,x_54,x_55,x_56,x_57,x_58,x_59,x_60,x_61,x_62,x_63,x_64,x_65,x_66,x_67,x_68,x_69,x_70,x_71,x_72,x_73,x_74,x_75,x_76,x_77,x_78,x_79), dp;\n")


print("ideal I = ")

for i in range(len(new_equations)):
    print(str(new_equations[i]).replace('x', 'x_') + ",")

print("x_0*x_0+ x_0, x_1*x_1+ x_1, x_2*x_2+ x_2, x_3*x_3+ x_3, x_4*x_4+ x_4, x_5*x_5+ x_5, x_6*x_6+ x_6, x_7*x_7+ x_7, x_8*x_8+ x_8, x_9*x_9+ x_9, x_10*x_10+ x_10, x_11*x_11+ x_11, x_12*x_12+ x_12, x_13*x_13+ x_13, x_14*x_14+ x_14, x_15*x_15+ x_15, x_16*x_16+ x_16, x_17*x_17+ x_17, x_18*x_18+ x_18, x_19*x_19+ x_19, x_20*x_20+ x_20, x_21*x_21+ x_21, x_22*x_22+ x_22, x_23*x_23+ x_23, x_24*x_24+ x_24, x_25*x_25+ x_25, x_26*x_26+ x_26, x_27*x_27+ x_27, x_28*x_28+ x_28, x_29*x_29+ x_29, x_30*x_30+ x_30, x_31*x_31+ x_31, x_32*x_32+ x_32, x_33*x_33+ x_33, x_34*x_34+ x_34, x_35*x_35+ x_35, x_36*x_36+ x_36, x_37*x_37+ x_37, x_38*x_38+ x_38, x_39*x_39+ x_39, x_40*x_40+ x_40, x_41*x_41+ x_41, x_42*x_42+ x_42, x_43*x_43+ x_43, x_44*x_44+ x_44, x_45*x_45+ x_45, x_46*x_46+ x_46, x_47*x_47+ x_47, x_48*x_48+ x_48, x_49*x_49+ x_49, x_50*x_50+ x_50, x_51*x_51+ x_51, x_52*x_52+ x_52, x_53*x_53+ x_53, x_54*x_54+ x_54, x_55*x_55+ x_55, x_56*x_56+ x_56, x_57*x_57+ x_57, x_58*x_58+ x_58, x_59*x_59+ x_59, x_60*x_60+ x_60, x_61*x_61+ x_61, x_62*x_62+ x_62, x_63*x_63+ x_63, x_64*x_64+ x_64, x_65*x_65+ x_65, x_66*x_66+ x_66, x_67*x_67+ x_67, x_68*x_68+ x_68, x_69*x_69+ x_69, x_70*x_70+ x_70, x_71*x_71+ x_71, x_72*x_72+ x_72, x_73*x_73+ x_73, x_74*x_74+ x_74, x_75*x_75+ x_75, x_76*x_76+ x_76, x_77*x_77+ x_77, x_78*x_78+ x_78, x_79*x_79+ x_79;\n")


print("print(groebner(I));")


# string cmd = read("singular_cmds/gb1.sing");
# execute(cmd);
