from hardposit import from_bits
import sys

class PositInstruction:
    def __init__(self, op, operand_a, operand_b, result):
        self.op = op
        self.op_a = operand_a
        self.op_b = operand_b
        self.res = result

    # ToDo change FF and 8,0 
    def verify(self):
        p_a = from_bits(int(self.op_a,16)&0xFF,8,0)
        p_b = from_bits(int(self.op_b,16)&0xFF,8,0)
        p_r = from_bits(int(self.res,16)&0xFF,8,0)
        match self.op:
            case "padd":
                p_c = p_a+p_b
            case "pmul":
                p_c = p_a*p_b
            case _:
                print("[ERROR] Unrecognized op "+self.op)
                exit(-1)
        assert(p_c==p_r)
        return p_c == p_r


    def __str__(self):
        return "OP: "+self.op+"\top_a: "+self.op_a+"\top_b: "+self.op_b+"\tresult: "+self.res

# Pline array in the format:
# ['913240000', '22814', '000015a8', 'd4c5850b', 'padd', 'x10,', 'x11,', 'x12', 'x10=0000008f', 'x11:0000008f', 'x12:00000006']
def extract_ins(pline):
    if(len(pline) < 11):
        print("[ERROR] Pline non in the correct format")
        exit(-1)
    op = pline[4];
    operand_a = pline[9].split(":")[1]
    operand_b = pline[10].split(":")[1]
    result = pline[8].split("=")[1]

    ins = PositInstruction(op,operand_a,operand_b,result)
    return ins

def parse_line(line):
    stripped_line = ' '.join(line.split())
    splitted_line = stripped_line.split(" ")
    if("padd" in splitted_line or "pmul" in splitted_line):
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

def validate_output(parsed_insns):
    for ins in parsed_insns:
        print(ins)
        ins.verify()
    
# Read input file from sys arg
if len(sys.argv) < 2:
    print("Usage: "+sys.argv[0]+" [tracer file]")
    exit(-1)


trace = sys.argv[1];

insns = parse_insns(trace)
validate_output(insns)



