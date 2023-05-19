#ifndef ch8
#define ch8
#include <iostream>   
#include <string>

class chip8 {
private:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I; // stores memory address; rightmost  12 bits used ; 16bits
    unsigned short pc;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];
public:
    unsigned char gfx[64 * 32];
    unsigned char delay_timer; //8bit
    unsigned char sound_timer;
    void initialize();
    void loadGamec(const char * filename);
    void loadGame(std::string path);
    void loadRom(char const* filename);
    void emulateCycle();
    int  drawflag;
    void setKeys(int k, int ud);
};
#endif ch8