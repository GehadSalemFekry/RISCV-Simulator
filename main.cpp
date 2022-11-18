 #include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

int reg[32];
int pc = 0;

map<string, int> reg_to_num = {{"zero", 0}, {"ra", 1}, {"sp", 2}, {"gp", 3}, {"tp", 4}, {"t0", 5}, {"t1", 6}, {"t2", 7},
                               {"s0", 8}, {"s1", 9}, {"a0", 10}, {"a1", 11}, {"a2", 12}, {"a3", 13}, {"a4", 14}, {"a5", 15},
                               {"a6", 16}, {"a7", 17}, {"s2", 18}, {"s3", 19}, {"s4", 20}, {"s5", 21}, {"s6", 22}, {"s7", 23},
                               {"s8", 24}, {"s9", 25}, {"s10", 26}, {"s11", 27}, {"t3", 28}, {"t4", 29}, {"t5", 30}, {"t6", 31}};

bool isFound(vector<string> data, string target) {
    return find(data.begin(), data.end(), target) != data.end();
}

int getRegNumber(string reg_name) {
    if (reg_name[0] == 'x') return stoi(reg_name.substr(1, reg_name.size() - 1));
    return reg_to_num[reg_name];
}

void getInstruction(vector<string> &data) {
    string instruction = data[0];

    vector<string> r_type = {"add", "sub", "sll", "slt", "sltu", "xor", "srl", "sra", "or", "and"};
    if (isFound(r_type, instruction)) {
        int rd = getRegNumber(data[1]), rs1 = getRegNumber(data[2]), rs2 = getRegNumber(data[3]);

        if (instruction == r_type[0]) { // add
            reg[rd] = reg[rs1] + reg[rs2];
        } else if (instruction == r_type[1]) { // sub
            reg[rd] = reg[rs1] - reg[rs2];
        } else if (instruction == r_type[2]) { // sll
            reg[rd] = reg[rs1] << reg[rs2];  
        } else if (instruction == r_type[3]) { // slt
            reg[rd] = (reg[rs1] < reg[rs2]) ? 1 : 0;
        } else if (instruction == r_type[4]) { // sltu (what is this!!)
            reg[rd] = ((unsigned int)reg[rs1] < (unsigned int)reg[rs2]) ? 1 : 0;
        } else if (instruction == r_type[5]) { // xor
            reg[rd] = reg[rs1] ^ reg[rs2];
        } else if (instruction == r_type[6]) { // srl  

            if (reg[rs1] < 0 && reg[rs2]) {
                reg[rd] = (long long)(pow(2, 32) + reg[rs1]) >> reg[rs2];
                if (!reg[rd] && shift < 0)
                    reg[rd] = pow(2, -reg[rs2]) - 1;
            }
            else reg[rd] = reg[rs1] >> reg[rs2];

        } else if (instruction == r_type[7]) { // sra
            reg[rd] = reg[rs1] >> reg[rs2];
        } else if (instruction == r_type[8]) { // or
            reg[rd] = reg[rs1] | reg[rs2];
        } else if (instruction == r_type[9]) { // and
            reg[rd] = reg[rs1] & reg[rs2];
        }

        if (rd == 0) reg[rd] = 0;
    }
    
    vector<string> i_type = {"addi", "slli","slti","sltiu","xori","srli","srai","ori", "andi", "jalr", "lb", "lbu", "lh", "lhu", "lw"};
    
    else if (isFound(i_type, instruction)) {
        int rd = getRegNumber(data[1]), rs1 = getRegNumber(data[2]), imm = stoi(data[3]);

        if (instruction == i_type[0]) { // addi
            reg[rd] = reg[rs1] + imm;
        }else if (instruction == i_type[1]) { // slli
            reg[rd] = reg[rs1] << imm;
        } else if (instruction == i_type[2]) { // slti
            reg[rd] = (reg[rs1] < imm) ? 1 : 0;
        } else if (instruction == i_type[3]) { // sltiu (what is this!!)
            reg[rd] = ((unsigned int)reg[rs1] < (unsigned int)imm) ? 1 : 0;
        } else if (instruction == i_type[4]) { // xori
            reg[rd] = reg[rs1] ^ imm;
        } else if (instruction == i_type[5]) { // srli
            reg[rd] = reg[rs1] >> imm;
        } else if (instruction == i_type[6]) { // srai (what is this!!)
            reg[rd] = reg[rs1] >> imm;
        } else if (instruction == i_type[7]) { // ori
            reg[rd] = reg[rs1] | imm;
        } else if (instruction == i_type[8]) { // andi
            reg[rd] = reg[rs1] & imm;
            
            
            
            
        } else if (instruction == i_type[9]) { // jalr hena w ta7t lessa
            pc = reg[rs1] + imm;
        } else if (instruction == i_type[10]) { // lb
            
        } else if (instruction == i_type[11]) { // lbu
            
        } else if (instruction == i_type[12]) { // lh
            
        } else if (instruction == i_type[13]) { // lhu
            
        } else if (instruction == i_type[14]) { // lw
            
        }

        if (rd == 0) reg[rd] = 0;
    }

    vector<string> SB_type = {"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU"};
    else if (isFound(SB_type, instruction)) {
        int rs1 = getRegNumber(data[1]), rs2 = getRegNumber(data[2]); 
        //string label = data[3];
        long long counter;
        if (instruction == SB_type[0]) {
            if (reg[rs1] == reg[rs2])
                pc += counter;
        }
        else if (instruction == SB_type[1]) {
            if (reg[rs1] != reg[rs2])
                pc += counter;
        }
        else if (instruction == SB_type[2]) {
            if (reg[rs1] < reg[rs2])
                pc += counter;
        }
        else if (instruction == SB_type[3]) {
            if (reg[rs1] >= reg[rs2])
                pc += counter;
        }
        else if (instruction == SB_type[4]) {
            if (((reg[rs1] % long long(pow(2, 31))) + pow(2, 31) * (bool(reg[rs1] < 0))) < ((reg[rs2] % long long(pow(2, 31))) + pow(2, 31) * (bool(reg[rs2] < 0))))
                pc += counter;
        }
        
    }

    if (rd == 0) reg[rd] = 0;  //__ overall for all calls
}



vector<string> separate(string line) {
    vector<string> words;
    string a = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] != ' ' || line[i] != ',') a += line[i];
        else if (a.size() > 0) words.push_back(a), a = "";
    }
    words.push_back(a);
    return words;
}

int main() {
    freopen("program.txt", "r", stdin);

    string line;
    while (getline(cin, line)) {
        vector<string> data = separate(line);
        getInstruction(data);
    }


    return 0;
}
