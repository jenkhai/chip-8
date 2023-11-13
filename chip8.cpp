#include <SDL2/SDL.h>
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
bool display[64][32];     // 64 by 32 pixels

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

void initDisplay() {
    for(int i=0;i<64;i++) {
        for(int j=0;j<32;j++) {
            display[i][j] = rand() % 2;
        } 
    }

    for(int j=0;j<32;j++) {
        for(int i=0;i<64;i++) {
            printf("%d", display[i][j]);
        }  printf("\n");
    }
}

void updateTexture(SDL_Texture* texture) {
    // todo: use lock and unlock to redraw pixels
    unsigned char* lockedPixels = nullptr;
    int pitch = 0;
    SDL_LockTexture( texture, nullptr, reinterpret_cast< void** >( &lockedPixels ), &pitch );

    for(int i=0;i<64;i++) {
        for(int j=0;j<32;j++) {
            const unsigned int offset = ( 64 * j * 4 ) + i * 4; // pointer arithmetic I copied from StackOverflow
            lockedPixels[ offset + 0 ] = (int)display[i][j]*255;  // b
            lockedPixels[ offset + 1 ] = (int)display[i][j]*255;  // g
            lockedPixels[ offset + 2 ] = (int)display[i][j]*255;  // r
            lockedPixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;        // a
        }
    }
    SDL_UnlockTexture( texture );
    initDisplay();
}

void sdl() {
    // Create a window
    const int W = 640*2;
    const int H = 320*2;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Handle initialization error
    }

    // Create and initialize window, renderer, texture
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    window = SDL_CreateWindow("CHIP-8",
            W/2,
            H/2,
            W,
            H,
            SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //SDL_RenderSetScale(renderer, 15.0, 15.0);

    SDL_Rect rect;
    rect.x = 160/15;
    rect.y = 80/15;
    rect.w = 64;
    rect.h = 32;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    // Main loop
    bool running = true;
    while(running) {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            if(event.type == SDL_MOUSEMOTION) {
                int xMouse, yMouse;
                SDL_GetMouseState(&xMouse,&yMouse);
                cout << "Mouse: " << xMouse << " " << yMouse << endl;
            }
        }

        
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Draw
        SDL_SetRenderDrawColor(renderer, 255,255,255, SDL_ALPHA_OPAQUE);
        //SDL_RenderDrawRect(renderer, &rect);

        updateTexture(texture);

        // Copy the texture to the renderer?
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Present 
        SDL_RenderPresent(renderer);

        SDL_Delay(1000);
    }
    
    // Clean up resources
   SDL_DestroyTexture(texture);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);

   SDL_Quit();
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *filepath = argv[1];                 
        readROM(filepath);
        //cycle();

        initDisplay();
        sdl();
    } else {
        printf("Error: only one argument expected.\n");
        exit(1);
    }
}
