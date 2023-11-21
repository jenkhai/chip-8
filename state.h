#ifndef STATE_H
#define STATE_H

#define START 0x200
#define   END 0xFFF

// sizeof(char) = 1 byte, unsigned char has range [0, 2^8-1 = 255]
extern unsigned char mem[4096];  // 4KB (writable) RAM
extern unsigned char V[16];      // 16x 8-bit variable registers
extern int PC;           // program counter
extern int end_of_ROM;      // 1 past the last index of the loaded program
extern bool display[64][32];     // 64 by 32 pixels

void initState();

#endif
