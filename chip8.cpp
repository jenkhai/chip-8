#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#define START 0x200
#define   END 0xFFF
using namespace std;

// sizeof(char) = 1 byte, unsigned char has range [0, 2^8-1 = 255]
unsigned char mem[4096];  // 4KB (writable) RAM
int PC = START;           // program counter
int end_of_ROM = -1;      // 1 past the last index of the loaded program

/* 
 * Load a .ch8 file into memory, starting from address 0x200
 */
void readROM(char filepath[]) {
    ifstream input(filepath, ios::binary);
    vector<unsigned char> buffer(istreambuf_iterator<char>(input), {});
    memset(mem, -1, sizeof(mem));
    if(!input) {} // error

    for(int i=0;i<buffer.size();i++) mem[START+i] = buffer[i];
     
    end_of_ROM = START+buffer.size();
    
    //for(int i=START;i<end_of_ROM;i++) {
    //    printf("%02x", mem[i]);
    //    if(i%2 ==1) printf(" ");
    //} printf("\n");

}

/*
 * Fetches the 2-byte instruction that lives at addresses (pc, pc+1)
 */
unsigned short fetch(int pc) {
    unsigned short instruction = (mem[pc] << 8) | mem[pc+1];
    printf("fetching: %04x at address 0x%x\n", instruction, pc);
    return instruction;
}

void decode(unsigned short instruction) {

}

/*
 * Run the fetch-decode-execute cycle
 * todo: add delay
 */ 
void cycle() {
    while(PC<end_of_ROM) {
        unsigned short instruction = fetch(PC); // fetch the current 2-byte instruction
        PC += 2;
        
        // decode();
        // execute();
    }
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *filepath = argv[1];                 
        readROM(filepath);
        cycle();
    } else {
        printf("Error: only one argument expected.\n");
        exit(1);
    }
}
