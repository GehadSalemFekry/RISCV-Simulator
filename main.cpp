#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

const int MEMORY_SIZE = 4 * 1024 * 1024 * 1024; // Not sure of the number
int mem[MEMORY_SIZE];
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

pair<int, int> getRegImm(string s) {
    int imm, reg;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            imm = stoi(s.substr(0, i));
            reg = getRegNumber(s.substr(i + 1, s.size() - i - 2));
        }
    }
    return {reg, imm};
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
        } else if (instruction == r_type[4]) { // sltu 
            reg[rd] = ((unsigned int)reg[rs1] < (unsigned int)reg[rs2]) ? 1 : 0;
        } else if (instruction == r_type[5]) { // xor
            reg[rd] = reg[rs1] ^ reg[rs2];
        } else if (instruction == r_type[6]) { // srl
            reg[rd] = reg[rs1] >> reg[rs2];
        } else if (instruction == r_type[7]) { // sra (what is this!!)
            reg[rd] = reg[rs1] >> reg[rs2];
        } else if (instruction == r_type[8]) { // or
            reg[rd] = reg[rs1] | reg[rs2];
        } else if (instruction == r_type[9]) { // and
            reg[rd] = reg[rs1] & reg[rs2];
        }

        if (rd == 0) reg[rd] = 0;
        pc += 4;
    }
    
    vector<string> i_type_arithmetic = {"addi", "slli","slti","sltiu","xori","srli","srai","ori", "andi", "jalr"};
    if (isFound(i_type_arithmetic, instruction)) {
        int rd = getRegNumber(data[1]), rs1 = getRegNumber(data[2]), imm = stoi(data[3]);

        if (instruction == i_type_arithmetic[0]) { // addi
            reg[rd] = reg[rs1] + imm;
        }else if (instruction == i_type_arithmetic[1]) { // slli
            reg[rd] = reg[rs1] << imm;
        } else if (instruction == i_type_arithmetic[2]) { // slti
            reg[rd] = (reg[rs1] < imm) ? 1 : 0;
        } else if (instruction == i_type_arithmetic[3]) { // sltiu 
            reg[rd] = ((unsigned int)reg[rs1] < (unsigned int)imm) ? 1 : 0;
        } else if (instruction == i_type_arithmetic[4]) { // xori
            reg[rd] = reg[rs1] ^ imm;
        } else if (instruction == i_type_arithmetic[5]) { // srli
            reg[rd] = reg[rs1] >> imm;
        } else if (instruction == i_type_arithmetic[6]) { // srai (what is this!!)
            reg[rd] = reg[rs1] >> imm;
        } else if (instruction == i_type_arithmetic[7]) { // ori
            reg[rd] = reg[rs1] | imm;
        } else if (instruction == i_type_arithmetic[8]) { // andi
            reg[rd] = reg[rs1] & imm;
        } else if (instruction == i_type_arithmetic[9]) { // jalr hena w ta7t lessa
            pc = reg[rs1] + imm;


        } 

        if (rd == 0) reg[rd] = 0;
    }

    vector<string> i_type_load = {"lb", "lbu", "lh", "lhu", "lw"};
    if (isFound(i_type_load, instruction)) {        
        int rd = getRegNumber(data[1]), rs1, offset;
        tie(rs1, offset) = getRegImm(data[2]);

        if (instruction == i_type_load[0]) { // lb
            
        } else if (instruction == i_type_load[1]) { // lbu
            
        } else if (instruction == i_type_load[2]) { // lh
            
        } else if (instruction == i_type_load[3]) { // lhu
            
        } else if (instruction == i_type_load[4]) { // lw
            
        }

        if (rd == 0) reg[rd] = 0;
        pc += 4;
    }

    vector<string> s_type = {"sb", "sh", "sw"};
    if (isFound(s_type, instruction)) {        
        int rs1 = getRegNumber(data[1]), rs2, offset;
        tie(rs2, offset) = getRegImm(data[2]);

        if (instruction == s_type[0]) { // sb 
            mem[reg[rs2] + offset] = reg[rs1] & 0xFF; 
        } else if (instruction == s_type[1]) { // sh
            mem[reg[rs2] + offset] = reg[rs1] & 0xFF;
            mem[reg[rs2] + offset + 1] = (reg[rs1] >> 8) & 0xFF;
        } else if (instruction == s_type[2]) { // sw
            mem[reg[rs2] + offset] = reg[rs1] & 0xFF;
            mem[reg[rs2] + offset + 1] = (reg[rs1] >> 8) & 0xFF;
            mem[reg[rs2] + offset + 2] = (reg[rs1] >> 16) & 0xFF;
            mem[reg[rs2] + offset + 3] = (reg[rs1] >> 24) & 0xFF;
        } 

        pc += 4;
    }

    vector<string> u_type = {"lui", "auipc"};
    if (isFound(u_type, instruction)) {
        int rs1 = getRegNumber(data[1]), imm = stoi(data[2]);

        if (instruction == u_type[0]) { // lui
            reg[rs1] = (imm << 12);
        } else if (instruction == u_type[1]) {// auipc
            reg[rs1] = pc + (imm << 12);
        }

        if (rd == 0) reg[rd] = 0;
        pc += 4;
    }
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
