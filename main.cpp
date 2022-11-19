#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

//Replace pow(2,32) with a const int value to reduce complexity in every function
//boolean found to check that this is even an instruction, so make it true when you get into a family.
//*if it was not found then cout "error in line _"
map<int, int> mem;
int reg[32];
int pc = 0;
map<string, int> labels;

map<string, int> reg_to_num = {{"zero", 0}, {"ra", 1}, {"sp", 2}, {"gp", 3}, {"tp", 4}, {"t0", 5}, {"t1", 6}, {"t2", 7},
                               {"s0", 8}, {"s1", 9}, {"a0", 10}, {"a1", 11}, {"a2", 12}, {"a3", 13}, {"a4", 14}, {"a5", 15},
                               {"a6", 16}, {"a7", 17}, {"s2", 18}, {"s3", 19}, {"s4", 20}, {"s5", 21}, {"s6", 22}, {"s7", 23},
                               {"s8", 24}, {"s9", 25}, {"s10", 26}, {"s11", 27}, {"t3", 28}, {"t4", 29}, {"t5", 30}, {"t6", 31}};

bool isFound(vector<string> data, string target) {
    return find(data.begin(), data.end(), target) != data.end();
}

string to_lower(string s) {
    for (int i = 0; i < s.size(); i++) if (isalpha(s[i])) s[i] = tolower(s[i]);
    return s;
}

int getRegNumber(string reg_name) {
    if (reg_name[0] == 'x') return stoi(reg_name.substr(1, reg_name.size() - 1));
    return reg_to_num[reg_name];
}

string decimalToBase(int n, int base = 2) { // By default base 2
    if (n == 0) return "0";

    string ans = "";
    while (n) {
        int rem = n % base;
        if (rem < 10) ans += (rem + '0');
        else ans += (rem + 'A');
    } 
    reverse(ans.begin(), ans.end());
    return ans;
}

string decimalToHexa(int n) {
    return "0x" + decimalToBase(n, 16);
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

vector<string> separate(string line, int num) {
    vector<string> words;
    string a = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] != ' ' && line[i] != ',' && line[i] != ':' && line[i] != '\t') a += line[i];
        else if (a.size() > 0) {
            if (line[i] == ':')
                labels[a] = num;
            else words.push_back(a);
            a = "";
        }
    }
    if (a.size()) words.push_back(a);

    return words;
}

void printContent() {
    cout << "Program Counter: " << pc << "\n";

    cout << "--------------------- Registers Content: ----------------------\n"; 
    for (int i = 0; i < 32; i++) {
        cout << "x" << i << ": " << reg[i] << "\t";
        if ((i + 1) % 8 == 0) cout << "\n";
    }
    cout << "---------------------- Memory Content: ------------------------\n";
    cout << "\n";

    int i = 0;
    for (auto u : mem) {
        cout << u.first << ": " << u.second << "\t";
        i++;
        if (i % 8 == 0) cout << "\n";
    }
    cout << "\n---------------------------------------------------------------\n";
}

void getInstruction(vector<string> &data) {
    //if (!data.size()) return;
    cout <<"(" << data[0] << ")hereeeeee ya ....";
    cout << to_lower(data[0]) << "\n";
    string instruction = to_lower(data[0]);
    cout << instruction << "\n";
    cout << "here1\n";

    vector<string> halt = {"ebreak", "ecall", "fence"}; // 3
    if (isFound(halt, instruction)) {
        printContent();
        exit(0); // close the program, not to allow any further operations
    }

    int rd = getRegNumber(data[1]);

    vector<string> r_type = {"add", "sub", "sll", "slt", "sltu", "xor", "srl", "sra", "or", "and"}; // 10
    if (isFound(r_type, instruction)) {
        int rs1 = getRegNumber(data[2]), rs2 = getRegNumber(data[3]);

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

            if (reg[rs1] < 0 && reg[rs2]) { // To be fixed  //Fixed
                reg[rd] = (long long)(pow(2, 32) + reg[rs1]) >> reg[rs2];
                if (!reg[rd] && reg[rs2] < 0)
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

        pc += 4;
    }
    
    cout << "here2\n";

    vector<string> i_type_arithmetic = {"addi", "slli", "slti", "sltiu", "xori", "srli", "srai", "ori", "andi"}; // 9
    if (isFound(i_type_arithmetic, instruction)) {
        int rs1 = getRegNumber(data[2]), imm = stoi(data[3]);

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
        } else if (instruction == i_type_arithmetic[5]) { // srli  //Fixed
            if (reg[rs1] < 0 && imm) {
                reg[rd] = (long long)(pow(2, 32) + reg[rs1]) >> imm;
                if (!reg[rd] && imm < 0)
                    reg[rd] = pow(2, -imm) - 1; 
            }
            else reg[rd] = reg[rs1] >> imm;

        } else if (instruction == i_type_arithmetic[6]) { // srai  //Correct
            reg[rd] = reg[rs1] >> imm;
        } else if (instruction == i_type_arithmetic[7]) { // ori
            reg[rd] = reg[rs1] | imm;
        } else if (instruction == i_type_arithmetic[8]) { // andi
            reg[rd] = reg[rs1] & imm;
        } 

        pc += 4;
    }

    cout << "here3\n";


    vector<string> i_type_load = {"lb", "lbu", "lh", "lhu", "lw", "jalr"}; // 5
    if (isFound(i_type_load, instruction)) {        
        int rs1, offset; tie(rs1, offset) = getRegImm(data[2]);

        if (instruction == i_type_load[0]) { // lb
            reg[rd] = (int32_t)((int8_t)mem[reg[rs1] + offset]);
        } else if (instruction == i_type_load[1]) { // lbu
            reg[rd] = (uint32_t)mem[reg[rs1] + offset];
        } else if (instruction == i_type_load[2]) { // lh
            reg[rd] = (int32_t)((int16_t)mem[reg[rs1] + offset] | (mem[reg[rs1] + offset + 1] << 8));
        } else if (instruction == i_type_load[3]) { // lhu
            reg[rd] = (uint32_t)mem[reg[rs1] + offset] | (mem[reg[rs1] + offset + 1] << 8);
        } else if (instruction == i_type_load[4]) { // lw
            reg[rd] = mem[reg[rs1] + offset] | (mem[reg[rs1] + offset + 1] << 8) | (mem[reg[rs1] + offset + 2] << 16) | (mem[reg[rs1] + offset + 3] << 24);
        } else if (instruction == i_type_load[5]) { // jalr
            //cout << offset << " " << reg[rs1] << " " << pc << "\n";
            reg[rd] = pc + 4;
            pc = reg[rs1] + offset;
        }
        if (instruction != i_type_load[5]) pc += 4;
    }

    cout << "here4\n";

    vector<string> s_type = {"sb", "sh", "sw"}; // 3
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
    cout << "here5\n";

    vector<string> sb_type = {"beq", "bne", "blt", "bge", "bltu", "bgeu"}; // 6
    if (isFound(sb_type, instruction)) {
        int rs1 = rd, rs2 = getRegNumber(data[2]);
        int counter = labels[data[3]] - pc;  

        if (instruction == sb_type[0]) { // beq
            if (reg[rs1] == reg[rs2])
                pc += counter;
            else pc += 4;
        }
        else if (instruction == sb_type[1]) { // bne
            if (reg[rs1] != reg[rs2])
                pc += counter;
            else pc += 4;
        }
        else if (instruction == sb_type[2]) { // blt
            if (reg[rs1] < reg[rs2])
                pc += counter;
            else pc += 4;
        }
        else if (instruction == sb_type[3]) { // bge
            if (reg[rs1] >= reg[rs2])
                pc += counter;
            else pc += 4;
        }
        else if (instruction == sb_type[4]) { // bltu
            //if ((reg[rs1] % (1LL << 31)) + ((1LL << 31) * (reg[rs1] < 0)) < (reg[rs2] % (1LL << 31)) + ((1LL << 31) * reg[rs2] < 0))
            if ((unsigned int)reg[rs1] < (unsigned int)reg[rs2])
                pc += counter;
            else pc += 4;
        }
        else if (instruction == sb_type[5]) { // bgeu
            if ((unsigned int)reg[rs1] >= (unsigned int)reg[rs2])
                pc += counter;
            else pc += 4;
        }
    }
    cout << "here6\n";


    vector<string> u_type = {"lui", "auipc"}; // 3
    if (isFound(u_type, instruction)) {
        int imm = stoi(data[2]);

        if (instruction == u_type[0]) { // lui
            reg[rd] = (imm << 12);
            pc += 4;
        } else if (instruction == u_type[1]) { // auipc
            reg[rd] = pc + (imm << 12);
            pc += 4;
        } 
    }

    cout << "here777777\n";

    if (instruction == "jal") { // jal
        cout << "here - ya rab ne5las\n";

        int counter = labels[data[2]] - pc;
        cout << counter << " " << labels[data[2]] << " " << pc << "\n";

        reg[rd] = pc + 4;
        cout << counter << " " << labels[data[2]] << " " << pc << "\n";

        pc += counter;
        cout << counter << " " << labels[data[2]] << " " << pc << "\n";

    }


    if (rd == 0) reg[rd] = 0;  //__ overall for all calls

    printContent();
}

int main() {
    ifstream fin_data, fin_prog;

    fin_data.open("fibonacci_data.txt");
    fin_prog.open("fibonacci_program.txt"); // We can have it user input

    freopen("fibonacci_output.txt", "w", stdout);

    int address, data;

    fin_data >> pc;
    while (fin_data >> address >> data) { // store byte by byte in the memory
        mem[address] = data & 0xFF;
        mem[address + 1] = (data >> 8) & 0xFF;
        mem[address + 2] = (data >> 16) & 0xFF;
        mem[address + 3] = (data >> 24) & 0xFF;
    }

    string line;
    map<int, vector<string>> program;

    //storing the program and initializing the labels vector with the addresses without excuting any instruction
    int init_add = pc;
    while (getline(fin_prog, line)) {
        cout << line << "\n";
        program[init_add] = separate(line, init_add);
        for (auto u : program[init_add]) cout << u << " ";
        cout << "\n";
        init_add += 4;
    }

    fin_data.close();
    fin_prog.close();

    //looping on the program instructions to excute them
    int end = 4 * program.size() + pc;
    while (pc < end) {
        for (auto u : program[pc]) cout << u << " ";
        getInstruction(program[pc]);
    }

    return 0;
}