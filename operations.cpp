#include "state.h"

/*
 * 00E0: Clear screen
 */
void clear_screen() {
    for(int i=0;i<64;i++) {
        for(int j=0;j<32;j++) {
            display[i][j] = 0;
        }
    }
}   

/*
 * 6XNN: Set
 */
void set_register(int x, unsigned char val) {
    V[x] = val;
}

/*
 * DXYN: Draw
 */
void update_display(int x, int y, int n) {
    int i = V[x], j = V[y];
}
