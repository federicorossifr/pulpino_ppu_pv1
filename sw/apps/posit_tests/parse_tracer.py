from hardposit import from_bits
import sys
from struct import pack,unpack
posit_insns = [
    "pmul",
    "padd",
    "pcvt.f",
    "fcvt.p",
    "pdiv",
    "psub"
]

# Dictionary of occurences of posit instructions
posit_insns_dict = {
    "pmul":0,
    "padd":0,
    "pcvt.f":0,
    "fcvt.p":0,
    "pdiv":0,
    "psub":0
}

# Dictionary of cumulative errors of posit instructions compared to float
posit_insns_error_dict = {
    "pmul":0,
    "padd":0,
    "pcvt.f":0,
    "fcvt.p":0,
    "pdiv":0,
    "psub":0
}

def add_error(ins, error):
    posit_insns_error_dict[ins] += error

def print_average_errors():
    for ins in posit_insns_dict:
        if posit_insns_dict[ins] != 0:
            print(ins+": "+str(posit_insns_error_dict[ins]/posit_insns_dict[ins]))

# Dictionary of passed test of posit instructions
posit_insns_success_dict = {
    "pmul":0,
    "padd":0,
    "pcvt.f":0,
    "fcvt.p":0,
    "pdiv":0,
    "psub":0
}
def increase_insn_dict(ins):
    posit_insns_dict[ins] += 1

def increase_insn_test_dict(ins):
    posit_insns_success_dict[ins] += 1

def print_insn_dict():
    for ins in posit_insns_dict:
        print(ins+": "+str(posit_insns_dict[ins]))

def print_percent_success():
    for ins in posit_insns_dict:
        if posit_insns_dict[ins] != 0:
            print(ins+": "+str(100*posit_insns_success_dict[ins]/posit_insns_dict[ins])+"%")

def float_from_bin(bin):
    pf = pack('!I', bin);
    return unpack('!f', pf)[0];

def bin_from_float(float):
    pi = pack('!f', float);
    return unpack('!I', pi)[0];

class PositInstruction:
    def __init__(self, op, operand_a, operand_b, result):
        self.op = op
        self.op_a = operand_a
        self.op_b = operand_b
        self.res = result

    def verify_binary_op(self,n,es):
        # Gen mask depending on n
        mask = (1 << n)-1;
        p_a = from_bits(int(self.op_a,16)&mask,n,es)
        p_b = from_bits(int(self.op_b,16)&mask,n,es)
        p_r = from_bits(int(self.res,16)&mask,n,es)
        match self.op:
            case "padd":
                p_c = p_a+p_b
                p_fl = p_a.eval() + p_b.eval()
            case "pmul":
                p_c = p_a*p_b
                p_fl = p_a.eval() * p_b.eval()
            case "pdiv":
                p_c = p_a/p_b
                p_fl = p_a.eval() / p_b.eval()
            case "psub":
                p_c = p_a-p_b
                p_fl = p_a.eval() - p_b.eval()
            case _:
                print("[ERROR] Unrecognized op "+self.op)
                exit(-1)
        if(p_c != p_r):
            print(self)
            print(self.op,p_c.eval(),p_r.eval(),p_c==p_r)  
        else:      
            increase_insn_test_dict(self.op)
        if(self.op == "pdiv"):
            print(p_a.eval(),p_b.eval(),p_c.eval(), p_fl);
        try:
            add_error(self.op, abs( (p_c.eval() - p_fl)/p_fl ))
        except ZeroDivisionError:
            pass
        return p_c == p_r


    def verify_unary_op(self,n,es):
        # Gen mask depending on n
        mask = (1 << n)-1;        
        match self.op:
            case "pcvt.f":
                p_a = from_bits(int(self.op_a,16)&mask,n,es).eval()
                p_c = float_from_bin(int(self.res,16))
            case "fcvt.p":
                p_c = from_bits(int(self.res,16)&mask,n,es).eval()
                p_a = float_from_bin(int(self.op_a,16))
            case _:
                print("[ERROR] Unrecognized op "+self.op)
                exit(-1)
        if p_c != p_a:
            print(self)
            print(self.op,p_c,p_a,p_c==p_a)
        else:
            increase_insn_test_dict(self.op)
        return p_c == p_a



    # ToDo change FF and 8,0 
    def verify(self,n,es):
        match self.op:
            case "padd" | "pmul" | "pdiv" | "psub":
                increase_insn_dict(self.op)
                return self.verify_binary_op(n,es)
            case "pcvt.f" | "fcvt.p":
                increase_insn_dict(self.op)
                return self.verify_unary_op(n,es)
            case _:
                print("[ERROR] Unrecognized op "+self.op)
                exit(-1)



    def __str__(self):
        if self.op_b == None:
            return "OP: "+self.op+"\top_a: "+self.op_a+"\tresult: "+self.res
        return "OP: "+self.op+"\top_a: "+self.op_a+"\top_b: "+self.op_b+"\tresult: "+self.res

# Pline array in the format:
# ['913240000', '22814', '000015a8', 'd4c5850b', 'padd', 'x10,', 'x11,', 'x12', 'x10=0000008f', 'x11:0000008f', 'x12:00000006']
# Or
# ['86360000', '2142', '0000162c', 'd205850b', 'pcvt.f', 'x10,', 'x11,', 'x0', 'x10=40000000', 'x11:00000060']
def extract_ins(pline):
    op = pline[4];
    if len(pline) < 11 and (op != "pcvt.f" and op != "fcvt.p"):
        print("[ERROR] Pline non in the correct format")
        exit(-1)
    operand_a = pline[9].split(":")[1]
    if(op != "pcvt.f" and op != "fcvt.p"):
        operand_b = pline[10].split(":")[1]
    else:
        operand_b = None

    result = pline[8].split("=")[1]

    ins = PositInstruction(op,operand_a,operand_b,result)
    return ins

def is_posit_instr(splitted_line):
    for ins in posit_insns:
        if ins in splitted_line:
            return True
    return False

def parse_line(line):
    stripped_line = ' '.join(line.split())
    splitted_line = stripped_line.split(" ")
    if is_posit_instr(splitted_line):
        return extract_ins(splitted_line)
    return None
     
def parse_insns(tracefile):
    parsed_insns =[]
    with open(trace) as f:
        while (line := f.readline().rstrip()):
            insn = parse_line(line);
            if insn != None:
                parsed_insns.append(parse_line(line))
    return parsed_insns 

def validate_output(parsed_insns,n,es):
    for ins in parsed_insns:
        #print(ins)
        ins.verify(n,es)
    
# Read input file from sys arg
if len(sys.argv) < 4:
    print("Usage: "+sys.argv[0]+" [tracer file]" + " [N]" + " [ES]")
    exit(-1)


trace = sys.argv[1];
N = int(sys.argv[2]);
ES = int(sys.argv[3]);
insns = parse_insns(trace)
validate_output(insns,N,ES)
print_insn_dict()
print_percent_success()
print_average_errors()
