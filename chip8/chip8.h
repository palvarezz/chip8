#include <iostream>   
class chip8 {
private:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I; // stores memory address; rightmost  12 bits used ; 16bits
    unsigned short pc;
    
    unsigned char delay_timer; //8bit
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];
public:
    unsigned char gfx[64 * 32];
    void initialize();
    void loadGame(const char * file);
    void emulateCycle();
    int  drawflag;
    void setKeys(int k, int ud);
};